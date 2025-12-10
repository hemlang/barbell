#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int n = argc > 1 ? atoi(argv[1]) : 1000000;

    // Create temp file
    char filename[] = "/tmp/barbell_file_io_XXXXXX";
    int fd = mkstemp(filename);
    if (fd < 0) {
        perror("mkstemp");
        return 1;
    }
    close(fd);

    // Write n bytes
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen write");
        return 1;
    }

    unsigned char *buf = malloc(n);
    for (int i = 0; i < n; i++) {
        buf[i] = (unsigned char)(i % 256);
    }
    fwrite(buf, 1, n, f);
    fclose(f);

    // Read back and sum
    f = fopen(filename, "rb");
    if (!f) {
        perror("fopen read");
        return 1;
    }

    size_t bytes_read = fread(buf, 1, n, f);
    fclose(f);

    long sum = 0;
    for (size_t i = 0; i < bytes_read; i++) {
        sum += buf[i];
    }

    // Cleanup
    free(buf);
    unlink(filename);

    printf("%ld\n", sum);
    return 0;
}
