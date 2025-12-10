#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Generate test text with patterns to match
char* generate_text(int iterations) {
    const char* base = "The quick brown fox jumps over the lazy dog. "
                       "Email: user123@example.com Phone: 555-1234 "
                       "Date: 2024-01-15 Price: $99.99 "
                       "URL: https://www.example.com/path?query=value "
                       "IP: 192.168.1.1 Code: ABC-123-XYZ\n";
    size_t base_len = strlen(base);
    size_t total_len = base_len * iterations;

    char* text = malloc(total_len + 1);
    if (!text) return NULL;

    char* ptr = text;
    for (int i = 0; i < iterations; i++) {
        memcpy(ptr, base, base_len);
        ptr += base_len;
    }
    *ptr = '\0';

    return text;
}

// Count all matches of a pattern in text
int count_matches(const char* text, const char* pattern) {
    regex_t regex;
    regmatch_t match;
    int count = 0;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return 0;
    }

    const char* ptr = text;
    while (regexec(&regex, ptr, 1, &match, 0) == 0) {
        count++;
        ptr += match.rm_eo;
        if (*ptr == '\0') break;
    }

    regfree(&regex);
    return count;
}

int main(int argc, char* argv[]) {
    int iterations = argc > 1 ? atoi(argv[1]) : 10000;

    char* text = generate_text(iterations);
    if (!text) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    // Various regex patterns to test different matching scenarios
    const char* patterns[] = {
        "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}",  // Email
        "[0-9]{3}-[0-9]{4}",                                 // Phone
        "[0-9]{4}-[0-9]{2}-[0-9]{2}",                        // Date
        "\\$[0-9]+\\.[0-9]{2}",                              // Price
        "https?://[a-zA-Z0-9.-]+/[a-zA-Z0-9/?=&._-]*",       // URL
        "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}", // IP
        "[A-Z]{3}-[0-9]{3}-[A-Z]{3}",                        // Code
        "\\b[a-z]{4,}\\b"                                    // 4+ letter words
    };
    int num_patterns = sizeof(patterns) / sizeof(patterns[0]);

    int total_matches = 0;
    for (int i = 0; i < num_patterns; i++) {
        total_matches += count_matches(text, patterns[i]);
    }

    printf("%d\n", total_matches);

    free(text);
    return 0;
}
