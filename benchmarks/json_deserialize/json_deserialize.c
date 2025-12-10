// JSON deserialization benchmark - C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Simple struct to deserialize into
typedef struct {
    int id;
    char name[64];
    double value;
    int active;
    int tags[5];
} Record;

// Simple JSON parser (minimal, for benchmark purposes)
static const char *skip_whitespace(const char *p) {
    while (*p && isspace(*p)) p++;
    return p;
}

static const char *parse_string(const char *p, char *out, size_t max_len) {
    if (*p != '"') return NULL;
    p++;
    size_t i = 0;
    while (*p && *p != '"' && i < max_len - 1) {
        out[i++] = *p++;
    }
    out[i] = '\0';
    if (*p == '"') p++;
    return p;
}

static const char *parse_number(const char *p, double *out) {
    char *end;
    *out = strtod(p, &end);
    return end;
}

static const char *parse_int_val(const char *p, int *out) {
    char *end;
    *out = (int)strtol(p, &end, 10);
    return end;
}

static const char *parse_bool(const char *p, int *out) {
    if (strncmp(p, "true", 4) == 0) {
        *out = 1;
        return p + 4;
    } else if (strncmp(p, "false", 5) == 0) {
        *out = 0;
        return p + 5;
    }
    return NULL;
}

int parse_record(const char *json, Record *rec) {
    const char *p = skip_whitespace(json);
    if (*p != '{') return 0;
    p++;

    while (*p && *p != '}') {
        p = skip_whitespace(p);
        if (*p == ',') { p++; continue; }
        if (*p != '"') break;

        // Parse key
        char key[32];
        p = parse_string(p, key, sizeof(key));
        if (!p) return 0;

        p = skip_whitespace(p);
        if (*p != ':') return 0;
        p++;
        p = skip_whitespace(p);

        // Parse value based on key
        if (strcmp(key, "id") == 0) {
            p = parse_int_val(p, &rec->id);
        } else if (strcmp(key, "name") == 0) {
            p = parse_string(p, rec->name, sizeof(rec->name));
        } else if (strcmp(key, "value") == 0) {
            p = parse_number(p, &rec->value);
        } else if (strcmp(key, "active") == 0) {
            p = parse_bool(p, &rec->active);
        } else if (strcmp(key, "tags") == 0) {
            if (*p != '[') return 0;
            p++;
            for (int i = 0; i < 5; i++) {
                p = skip_whitespace(p);
                p = parse_int_val(p, &rec->tags[i]);
                p = skip_whitespace(p);
                if (*p == ',') p++;
            }
            p = skip_whitespace(p);
            if (*p == ']') p++;
        }
        if (!p) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    // JSON string to parse
    const char *json_template = "{\"id\":%d,\"name\":\"benchmark_test\",\"value\":%.6f,\"active\":true,\"tags\":[1,2,3,4,5]}";
    char json_str[256];

    Record rec;
    long total_id = 0;

    for (int i = 0; i < n; i++) {
        // Generate slightly different JSON each iteration
        snprintf(json_str, sizeof(json_str), json_template, i, 3.14159 + (i % 100) * 0.001);
        parse_record(json_str, &rec);
        total_id += rec.id;
    }

    printf("%ld\n", total_id);
    return 0;
}
