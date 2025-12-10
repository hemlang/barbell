// Unicode normalization benchmark
// Performs manual UTF-8 string normalization operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test strings with combining characters and precomposed forms
// NFD: e + combining acute (U+0065 U+0301) vs NFC: é (U+00E9)
// NFD: n + combining tilde (U+006E U+0303) vs NFC: ñ (U+00F1)
// NFD: a + combining ring above (U+0061 U+030A) vs NFC: å (U+00E5)

// Common combining character mappings (base + combining -> precomposed)
typedef struct {
    unsigned int base;
    unsigned int combining;
    unsigned int composed;
} CompositionMapping;

static const CompositionMapping compositions[] = {
    {0x0065, 0x0301, 0x00E9},  // e + acute -> é
    {0x0045, 0x0301, 0x00C9},  // E + acute -> É
    {0x0061, 0x0301, 0x00E1},  // a + acute -> á
    {0x0041, 0x0301, 0x00C1},  // A + acute -> Á
    {0x006F, 0x0301, 0x00F3},  // o + acute -> ó
    {0x004F, 0x0301, 0x00D3},  // O + acute -> Ó
    {0x0075, 0x0301, 0x00FA},  // u + acute -> ú
    {0x0055, 0x0301, 0x00DA},  // U + acute -> Ú
    {0x0069, 0x0301, 0x00ED},  // i + acute -> í
    {0x0049, 0x0301, 0x00CD},  // I + acute -> Í
    {0x006E, 0x0303, 0x00F1},  // n + tilde -> ñ
    {0x004E, 0x0303, 0x00D1},  // N + tilde -> Ñ
    {0x0061, 0x030A, 0x00E5},  // a + ring -> å
    {0x0041, 0x030A, 0x00C5},  // A + ring -> Å
    {0x0063, 0x0327, 0x00E7},  // c + cedilla -> ç
    {0x0043, 0x0327, 0x00C7},  // C + cedilla -> Ç
    {0x0065, 0x0300, 0x00E8},  // e + grave -> è
    {0x0045, 0x0300, 0x00C8},  // E + grave -> È
    {0x0061, 0x0300, 0x00E0},  // a + grave -> à
    {0x0041, 0x0300, 0x00C0},  // A + grave -> À
    {0x006F, 0x0302, 0x00F4},  // o + circumflex -> ô
    {0x004F, 0x0302, 0x00D4},  // O + circumflex -> Ô
    {0x0075, 0x0308, 0x00FC},  // u + diaeresis -> ü
    {0x0055, 0x0308, 0x00DC},  // U + diaeresis -> Ü
    {0, 0, 0}
};

// Decode UTF-8 codepoint, return bytes consumed
int utf8_decode(const unsigned char *s, unsigned int *cp) {
    if (s[0] < 0x80) {
        *cp = s[0];
        return 1;
    } else if ((s[0] & 0xE0) == 0xC0) {
        *cp = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
        return 2;
    } else if ((s[0] & 0xF0) == 0xE0) {
        *cp = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
        return 3;
    } else if ((s[0] & 0xF8) == 0xF0) {
        *cp = ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);
        return 4;
    }
    *cp = s[0];
    return 1;
}

// Encode codepoint to UTF-8, return bytes written
int utf8_encode(unsigned int cp, unsigned char *out) {
    if (cp < 0x80) {
        out[0] = cp;
        return 1;
    } else if (cp < 0x800) {
        out[0] = 0xC0 | (cp >> 6);
        out[1] = 0x80 | (cp & 0x3F);
        return 2;
    } else if (cp < 0x10000) {
        out[0] = 0xE0 | (cp >> 12);
        out[1] = 0x80 | ((cp >> 6) & 0x3F);
        out[2] = 0x80 | (cp & 0x3F);
        return 3;
    } else {
        out[0] = 0xF0 | (cp >> 18);
        out[1] = 0x80 | ((cp >> 12) & 0x3F);
        out[2] = 0x80 | ((cp >> 6) & 0x3F);
        out[3] = 0x80 | (cp & 0x3F);
        return 4;
    }
}

// Find composition for base + combining
unsigned int find_composition(unsigned int base, unsigned int combining) {
    for (int i = 0; compositions[i].base != 0; i++) {
        if (compositions[i].base == base && compositions[i].combining == combining) {
            return compositions[i].composed;
        }
    }
    return 0;
}

// Find decomposition for precomposed character
int find_decomposition(unsigned int composed, unsigned int *base, unsigned int *combining) {
    for (int i = 0; compositions[i].base != 0; i++) {
        if (compositions[i].composed == composed) {
            *base = compositions[i].base;
            *combining = compositions[i].combining;
            return 1;
        }
    }
    return 0;
}

// Simple NFC normalization (compose combining sequences)
size_t normalize_nfc(const char *input, char *output, size_t outsize) {
    const unsigned char *in = (const unsigned char *)input;
    unsigned char *out = (unsigned char *)output;
    size_t out_pos = 0;
    size_t in_len = strlen(input);
    size_t i = 0;

    while (i < in_len && out_pos < outsize - 4) {
        unsigned int cp1, cp2;
        int len1 = utf8_decode(in + i, &cp1);

        // Check if next char is a combining character
        if (i + len1 < in_len) {
            int len2 = utf8_decode(in + i + len1, &cp2);
            unsigned int composed = find_composition(cp1, cp2);
            if (composed) {
                out_pos += utf8_encode(composed, out + out_pos);
                i += len1 + len2;
                continue;
            }
        }

        out_pos += utf8_encode(cp1, out + out_pos);
        i += len1;
    }

    output[out_pos] = '\0';
    return out_pos;
}

// Simple NFD normalization (decompose precomposed characters)
size_t normalize_nfd(const char *input, char *output, size_t outsize) {
    const unsigned char *in = (const unsigned char *)input;
    unsigned char *out = (unsigned char *)output;
    size_t out_pos = 0;
    size_t in_len = strlen(input);
    size_t i = 0;

    while (i < in_len && out_pos < outsize - 8) {
        unsigned int cp;
        int len = utf8_decode(in + i, &cp);

        unsigned int base, combining;
        if (find_decomposition(cp, &base, &combining)) {
            out_pos += utf8_encode(base, out + out_pos);
            out_pos += utf8_encode(combining, out + out_pos);
        } else {
            out_pos += utf8_encode(cp, out + out_pos);
        }

        i += len;
    }

    output[out_pos] = '\0';
    return out_pos;
}

int main(int argc, char *argv[]) {
    int iterations = argc > 1 ? atoi(argv[1]) : 100000;

    // Test string with mixed NFD and NFC characters
    // Contains: café (with é), naïve, résumé, coöperate, señor
    const char *test_nfd = "cafe\xCC\x81 na\xC3\xAFve re\xCC\x81sume\xCC\x81 coo\xCC\x88perate sen\xCC\x83or";
    const char *test_nfc = "caf\xC3\xA9 na\xC3\xAFve r\xC3\xA9sum\xC3\xA9 co\xC3\xB6perate se\xC3\xB1or";

    char buffer1[1024];
    char buffer2[1024];
    long total_len = 0;

    for (int i = 0; i < iterations; i++) {
        // NFC: compose combining sequences
        size_t len1 = normalize_nfc(test_nfd, buffer1, sizeof(buffer1));

        // NFD: decompose precomposed characters
        size_t len2 = normalize_nfd(test_nfc, buffer2, sizeof(buffer2));

        total_len += len1 + len2;
    }

    printf("%ld\n", total_len);
    return 0;
}
