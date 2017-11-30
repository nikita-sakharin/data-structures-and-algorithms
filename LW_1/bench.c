#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "radix_sort.h"

enum {
    KEY_BYTE_CNT = 1,
    STR_BYTE_CNT = 65
};

typedef struct {
    uint8_t key[KEY_BYTE_CNT];
    char string[STR_BYTE_CNT];
} TPair;

int DataCmp(const void *, const void *);

void RandArray(void *, size_t, size_t, size_t, size_t);

long double ExchangeSortTime(void *, size_t, size_t, void (*)(void *, size_t, size_t, int (*)(const void *, const void *)), int (*)(const void *, const void *));

long double LinearSortTime(void *, size_t, size_t, size_t, size_t, void (*)(void *, size_t, size_t, size_t, size_t), int (*)(const void *, const void *));

int main(void) {
    size_t n;
    scanf("%zu", &n);
    TPair *matrix = (TPair *) malloc(sizeof(TPair) * n), *copy = (TPair *) malloc(sizeof(TPair) * n);
    if (matrix == NULL || copy == NULL) {
        perror("");
        exit(EXIT_FAILURE);
    }

    srand((unsigned) time(NULL));
    RandArray(matrix, n, sizeof(TPair), 0u, KEY_BYTE_CNT);
    memcpy(copy, matrix, sizeof(TPair) * n);

    long double exchange = ExchangeSortTime(matrix, n, sizeof(TPair), qsort, DataCmp);
    long double linear = LinearSortTime(copy, n, sizeof(TPair), offsetof(TPair, key), KEY_BYTE_CNT, RadixSort, DataCmp);
    printf("exchange = %.15Lfs\n", exchange);
    printf("linear   = %.15Lfs\n", linear);

    free(matrix);
    free(copy);

    return 0;
}

int DataCmp(register const void *a, register const void *b)
{
    for (register size_t i = KEY_BYTE_CNT; i > 0u; i--) {
        if (*((uint8_t *) ((TPair *) a)->key + i - 1u) > *((uint8_t *) ((TPair *) b)->key + i - 1u)) {
            return 1;
        }
        else if (*((uint8_t *) ((TPair *) a)->key + i - 1u) < *((uint8_t *) ((TPair *) b)->key + i - 1u)) {
            return -1;
        }
    }

    return 0;
}

void RandArray(void *data, size_t count, size_t size, size_t val_offset, size_t val_size)
{
    if (data == NULL || !size || !val_size || val_offset + val_size > size) {
        printf("ERROR: Wrong arguments.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0u; i < count; i++) {
        for (size_t j = 0u; j < val_size; j++)
        {
            *((uint8_t *) data + i * size + j) = rand() % (UINT8_MAX + 1u);
        }
    }
}

long double ExchangeSortTime(void *data, size_t count, size_t size, void (*sort)(void *, size_t, size_t, int (*)(const void *, const void *)), int (*cmp)(const void *, const void *)) {
    clock_t begin = clock();
    sort(data, count, size, cmp);
    clock_t end = clock();
    for (size_t i = 1u; i < count; i++) {
        if (cmp((uint8_t *) data + i * size, (uint8_t *) data + (i - 1u) * size) < 0) {
            printf("ERROR: wrong sorting results.1\n");
        }
    }
    return (end - begin) / (long double) CLOCKS_PER_SEC;
}

long double LinearSortTime(void *data, size_t count, size_t size, size_t key_offset, size_t key_size, void (*sort)(void *, size_t, size_t, size_t, size_t), int (*cmp)(const void *, const void *)) {
    clock_t begin = clock();
    sort(data, count, size, key_offset, key_size);
    clock_t end = clock();
    for (size_t i = 1u; i < count; i++) {
        if (cmp((uint8_t *) data + i * size, (uint8_t *) data + (i - 1u) * size) < 0) {
            printf("ERROR: wrong sorting results.2\n");
        }
    }
    return (end - begin) / (long double) CLOCKS_PER_SEC;
}