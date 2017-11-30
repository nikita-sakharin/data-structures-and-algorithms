#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "radix_sort.h"

enum {
    KEY_BYTE_CNT = 16,
    STR_BYTE_CNT = 65
};

typedef struct {
    uint8_t key[KEY_BYTE_CNT];
    char string[STR_BYTE_CNT];
} TPair;

static const size_t BUFFER_START_SIZE = 64u;

int main(void)
{
    TPair *buffer = malloc(BUFFER_START_SIZE * sizeof(TPair));
    if (buffer == NULL) {
        fprintf(stdout, "ERROR: bad memory allocation.\n");
        exit(EXIT_SUCCESS);
    }

    size_t buffer_capacity = BUFFER_START_SIZE, buffer_count = 0u;

    for (; ; buffer_count++) {
        if (buffer_count == buffer_capacity) {
            buffer_capacity *= 2u;
            buffer = realloc(buffer, buffer_capacity * sizeof(TPair));
            if (buffer == NULL) {
                fprintf(stdout, "ERROR: bad memory allocation.\n");
                exit(EXIT_SUCCESS);
            }
        }
        int result = scanf("%16"SCNx64"%16"SCNx64"%*[\t ]%64[^\n]", (uint64_t *) &buffer[buffer_count].key[8u], (uint64_t *) buffer[buffer_count].key, buffer[buffer_count].string);
        if (result < 2) {
            break;
        }
    }

    if (buffer_count != buffer_capacity) {
        buffer = realloc(buffer, buffer_count * sizeof(TPair));
        if (buffer == NULL) {
            fprintf(stdout, "ERROR: bad memory allocation.\n");
            exit(EXIT_SUCCESS);
        }
        buffer_capacity = buffer_count;
    }

    RadixSort(buffer, buffer_count, sizeof(TPair), offsetof(TPair, key), KEY_BYTE_CNT);

    for (size_t i = 0u; i < buffer_count; i++) {
        printf("%016"PRIx64"%016"PRIx64"\t%s\n", *(uint64_t *) &buffer[i].key[8u], *(uint64_t *) buffer[i].key, buffer[i].string);
    }

    free(buffer);

    return 0;
}