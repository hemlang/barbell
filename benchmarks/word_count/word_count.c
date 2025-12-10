// Word count benchmark - C
// Count words and lines in large generated text (classic MapReduce task)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 100000;

    const char *phrase = "the quick brown fox jumps over the lazy dog\n";
    size_t phrase_len = strlen(phrase);

    // Generate text: repeat phrase n times
    size_t total_len = phrase_len * n;
    char *text = malloc(total_len + 1);
    if (!text) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char *ptr = text;
    for (int i = 0; i < n; i++) {
        memcpy(ptr, phrase, phrase_len);
        ptr += phrase_len;
    }
    *ptr = '\0';

    // Count words and lines
    long words = 0;
    long lines = 0;
    int in_word = 0;

    for (size_t i = 0; i < total_len; i++) {
        char c = text[i];
        if (c == '\n') {
            lines++;
        }
        if (isspace((unsigned char)c)) {
            in_word = 0;
        } else {
            if (!in_word) {
                words++;
                in_word = 1;
            }
        }
    }

    printf("%ld %ld\n", words, lines);
    free(text);
    return 0;
}
