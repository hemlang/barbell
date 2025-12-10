// JSON parse/serialize benchmark - C implementation
// Minimal JSON parser for benchmarking (handles objects, arrays, strings, numbers, bools, null)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { JSON_NULL, JSON_BOOL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT } JsonType;

typedef struct JsonValue JsonValue;
typedef struct JsonPair JsonPair;

struct JsonValue {
    JsonType type;
    union {
        int bool_val;
        double num_val;
        char *str_val;
        struct { JsonValue **items; int count; int capacity; } array;
        struct { JsonPair *pairs; int count; int capacity; } object;
    };
};

struct JsonPair {
    char *key;
    JsonValue *value;
};

static const char *json_input;
static int json_pos;

static void skip_whitespace(void) {
    while (json_input[json_pos] && isspace(json_input[json_pos])) json_pos++;
}

static JsonValue *parse_value(void);

static char *parse_string_raw(void) {
    if (json_input[json_pos] != '"') return NULL;
    json_pos++;
    int start = json_pos;
    while (json_input[json_pos] && json_input[json_pos] != '"') {
        if (json_input[json_pos] == '\\') json_pos++;
        json_pos++;
    }
    int len = json_pos - start;
    char *s = malloc(len + 1);
    memcpy(s, json_input + start, len);
    s[len] = '\0';
    json_pos++; // skip closing quote
    return s;
}

static JsonValue *parse_string(void) {
    JsonValue *v = malloc(sizeof(JsonValue));
    v->type = JSON_STRING;
    v->str_val = parse_string_raw();
    return v;
}

static JsonValue *parse_number(void) {
    JsonValue *v = malloc(sizeof(JsonValue));
    v->type = JSON_NUMBER;
    char *end;
    v->num_val = strtod(json_input + json_pos, &end);
    json_pos = end - json_input;
    return v;
}

static JsonValue *parse_array(void) {
    JsonValue *v = malloc(sizeof(JsonValue));
    v->type = JSON_ARRAY;
    v->array.items = NULL;
    v->array.count = 0;
    v->array.capacity = 0;
    json_pos++; // skip [
    skip_whitespace();
    while (json_input[json_pos] != ']') {
        if (v->array.count >= v->array.capacity) {
            v->array.capacity = v->array.capacity ? v->array.capacity * 2 : 8;
            v->array.items = realloc(v->array.items, v->array.capacity * sizeof(JsonValue*));
        }
        v->array.items[v->array.count++] = parse_value();
        skip_whitespace();
        if (json_input[json_pos] == ',') { json_pos++; skip_whitespace(); }
    }
    json_pos++; // skip ]
    return v;
}

static JsonValue *parse_object(void) {
    JsonValue *v = malloc(sizeof(JsonValue));
    v->type = JSON_OBJECT;
    v->object.pairs = NULL;
    v->object.count = 0;
    v->object.capacity = 0;
    json_pos++; // skip {
    skip_whitespace();
    while (json_input[json_pos] != '}') {
        if (v->object.count >= v->object.capacity) {
            v->object.capacity = v->object.capacity ? v->object.capacity * 2 : 8;
            v->object.pairs = realloc(v->object.pairs, v->object.capacity * sizeof(JsonPair));
        }
        JsonPair *p = &v->object.pairs[v->object.count++];
        p->key = parse_string_raw();
        skip_whitespace();
        json_pos++; // skip :
        skip_whitespace();
        p->value = parse_value();
        skip_whitespace();
        if (json_input[json_pos] == ',') { json_pos++; skip_whitespace(); }
    }
    json_pos++; // skip }
    return v;
}

static JsonValue *parse_value(void) {
    skip_whitespace();
    char c = json_input[json_pos];
    if (c == '"') return parse_string();
    if (c == '[') return parse_array();
    if (c == '{') return parse_object();
    if (c == 't') { json_pos += 4; JsonValue *v = malloc(sizeof(JsonValue)); v->type = JSON_BOOL; v->bool_val = 1; return v; }
    if (c == 'f') { json_pos += 5; JsonValue *v = malloc(sizeof(JsonValue)); v->type = JSON_BOOL; v->bool_val = 0; return v; }
    if (c == 'n') { json_pos += 4; JsonValue *v = malloc(sizeof(JsonValue)); v->type = JSON_NULL; return v; }
    return parse_number();
}

static void json_free(JsonValue *v) {
    if (!v) return;
    if (v->type == JSON_STRING) free(v->str_val);
    else if (v->type == JSON_ARRAY) {
        for (int i = 0; i < v->array.count; i++) json_free(v->array.items[i]);
        free(v->array.items);
    } else if (v->type == JSON_OBJECT) {
        for (int i = 0; i < v->object.count; i++) {
            free(v->object.pairs[i].key);
            json_free(v->object.pairs[i].value);
        }
        free(v->object.pairs);
    }
    free(v);
}

static int json_serialize(JsonValue *v, char *buf, int size) {
    int len = 0;
    switch (v->type) {
        case JSON_NULL: len = snprintf(buf, size, "null"); break;
        case JSON_BOOL: len = snprintf(buf, size, v->bool_val ? "true" : "false"); break;
        case JSON_NUMBER: len = snprintf(buf, size, "%g", v->num_val); break;
        case JSON_STRING: len = snprintf(buf, size, "\"%s\"", v->str_val); break;
        case JSON_ARRAY:
            len = snprintf(buf, size, "[");
            for (int i = 0; i < v->array.count; i++) {
                if (i > 0) len += snprintf(buf + len, size - len, ",");
                len += json_serialize(v->array.items[i], buf + len, size - len);
            }
            len += snprintf(buf + len, size - len, "]");
            break;
        case JSON_OBJECT:
            len = snprintf(buf, size, "{");
            for (int i = 0; i < v->object.count; i++) {
                if (i > 0) len += snprintf(buf + len, size - len, ",");
                len += snprintf(buf + len, size - len, "\"%s\":", v->object.pairs[i].key);
                len += json_serialize(v->object.pairs[i].value, buf + len, size - len);
            }
            len += snprintf(buf + len, size - len, "}");
            break;
    }
    return len;
}

// Test JSON data
static const char *TEST_JSON =
    "{\"name\":\"John Doe\",\"age\":30,\"active\":true,\"balance\":1234.56,"
    "\"address\":{\"street\":\"123 Main St\",\"city\":\"Springfield\",\"zip\":\"12345\"},"
    "\"tags\":[\"developer\",\"golang\",\"python\",\"rust\"],"
    "\"scores\":[95,87,92,88,91],\"metadata\":null}";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    char *output = malloc(4096);
    int total_len = 0;

    for (int i = 0; i < n; i++) {
        // Parse
        json_input = TEST_JSON;
        json_pos = 0;
        JsonValue *v = parse_value();

        // Serialize
        int len = json_serialize(v, output, 4096);
        total_len += len;

        // Free
        json_free(v);
    }

    printf("%d\n", total_len);
    free(output);
    return 0;
}
