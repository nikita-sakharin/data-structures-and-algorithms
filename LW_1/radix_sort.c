#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radix_sort.h"

const size_t TABLE_CAPACITY = 256u;

typedef unsigned char TByte;

#define BYTE_OFFSET(data, offset_in_elem, idx, size) ((TByte *) (data) + (offset_in_elem) + (idx) * (size))

void RadixSort(register void *data, size_t count, size_t size, size_t key_offset, size_t key_size) {

#   ifdef _RADIX_SORT_H_SAVE_
    if (data == NULL || !size || !key_size || key_offset + key_size > size) {
        fprintf(stdout, "ERROR: dereferencing NULL pointer.\n");
        exit(EXIT_SUCCESS);
    }
#   endif

    if (count < 2u) {
        return;
    }

    register void *buffer = malloc(count * size);
    if (buffer == NULL) {
        fprintf(stdout, "ERROR: bad memory allocation.\n");
        exit(EXIT_SUCCESS);
    }

    for (register size_t i = key_offset; i < key_size + key_offset; i++) {
        size_t match_table[TABLE_CAPACITY];
        for (register size_t j = 0u; j < TABLE_CAPACITY; j++) {
            match_table[j] = 0u;
        }

        for (register size_t j = 0u; j < count; j++) {
            if ((i - key_offset) % 2u) {
                match_table[*BYTE_OFFSET(buffer, i, j, size)]++;
            }
            else {
                match_table[*BYTE_OFFSET(data, i, j, size)]++;
            }
        }

        for (register size_t j = 1u; j < TABLE_CAPACITY; j++) {
            match_table[j] += match_table[j - 1u];
        }

        for (register size_t j = count; j > 0u; j--) {
            if ((i - key_offset) % 2u) {
                match_table[*BYTE_OFFSET(buffer, i, j - 1u, size)]--;
                memcpy(
                    BYTE_OFFSET(data, 0u, match_table[*BYTE_OFFSET(buffer, i, j - 1u, size)], size),
                    BYTE_OFFSET(buffer, 0u, j - 1u, size),
                    size
                );
            }
            else {
                match_table[*BYTE_OFFSET(data, i, j - 1u, size)]--;
                memcpy(
                    BYTE_OFFSET(buffer, 0u, match_table[*BYTE_OFFSET(data, i, j - 1u, size)], size),
                    BYTE_OFFSET(data, 0u, j - 1u, size),
                    size
                );
            }
        }
    }

    if (key_size % 2u) {
        memcpy(data, buffer, size * count);
    }

    free(buffer);
}