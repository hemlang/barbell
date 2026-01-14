#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TABLE_SIZE 10007

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[TABLE_SIZE];
} HashTable;

unsigned int hash(const char *key) {
    unsigned int h = 0;
    while (*key) {
        h = h * 31 + (unsigned char)(*key++);
    }
    return h % TABLE_SIZE;
}

void ht_insert(HashTable *ht, const char *key, int value) {
    unsigned int idx = hash(key);
    Entry *e = ht->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            e->value = value;
            return;
        }
        e = e->next;
    }
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = ht->buckets[idx];
    ht->buckets[idx] = new_entry;
}

int ht_get(HashTable *ht, const char *key, int *found) {
    unsigned int idx = hash(key);
    Entry *e = ht->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) {
            *found = 1;
            return e->value;
        }
        e = e->next;
    }
    *found = 0;
    return 0;
}

void ht_delete(HashTable *ht, const char *key) {
    unsigned int idx = hash(key);
    Entry *e = ht->buckets[idx];
    Entry *prev = NULL;
    while (e) {
        if (strcmp(e->key, key) == 0) {
            if (prev) {
                prev->next = e->next;
            } else {
                ht->buckets[idx] = e->next;
            }
            free(e->key);
            free(e);
            return;
        }
        prev = e;
        e = e->next;
    }
}

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    HashTable ht = {0};
    char key[32];
    long checksum = 0;

    // Insert n items
    for (int i = 0; i < n; i++) {
        sprintf(key, "key_%d", i);
        ht_insert(&ht, key, i * 2);
    }

    // Lookup all items
    for (int i = 0; i < n; i++) {
        sprintf(key, "key_%d", i);
        int found;
        int val = ht_get(&ht, key, &found);
        if (found) {
            checksum += val;
        }
    }

    // Delete half the items
    for (int i = 0; i < n; i += 2) {
        sprintf(key, "key_%d", i);
        ht_delete(&ht, key);
    }

    // Lookup remaining items
    for (int i = 0; i < n; i++) {
        sprintf(key, "key_%d", i);
        int found;
        int val = ht_get(&ht, key, &found);
        if (found) {
            checksum += val;
        }
    }

    gettimeofday(&end, NULL);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    fprintf(stderr, "TIME_MS:%.2f\n", elapsed_ms);

    printf("%ld\n", checksum);
    return 0;
}
