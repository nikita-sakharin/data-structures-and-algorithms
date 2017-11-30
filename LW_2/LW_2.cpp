#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "patricia_tree.h"

enum {
    STR_SIZE = 257u
};

#define STR_SIZE_ "256"

int main(void) {
    TPatriciaTree *tree = PatriciaTreeCreate(sizeof(uint64_t));
    if (tree == NULL) {
        printf("ERROR: malloc error.\n");
        exit(EXIT_SUCCESS);
    }

    char buffer[STR_SIZE];
    int buffer_size;

    while (scanf("%" STR_SIZE_"s%n", buffer, &buffer_size) != EOF) {
        uint64_t value;
        if (buffer[0u] == '+') {
            scanf("%*[\t ]");
            scanf("%" STR_SIZE_"s%n%" SCNu64, buffer, &buffer_size, &value);
            bool result = PatriciaTreeInsert(tree, buffer, (size_t) buffer_size, &value);
            if (result) {
                printf("OK\n");
            }
            else {
                printf("Exist\n");
            }
        }
        else if (buffer[0u] == '-') {
            scanf("%*[\t ]");
            scanf("%" STR_SIZE_"s%n", buffer, &buffer_size);
            bool result = PatriciaTreeDelete(tree, buffer, (size_t) buffer_size);
            if (result) {
                printf("OK\n");
            }
            else {
                printf("NoSuchWord\n");
            }
        }
        else if (isalpha(buffer[0u])) {
            bool result = PatriciaTreeFind(tree, buffer, (size_t) buffer_size, &value);
            if (result) {
                printf("OK: %" PRIu64"\n", value);
            }
            else {
                printf("NoSuchWord\n");
            }
        }
        else {
            scanf("%" STR_SIZE_"s", buffer);
            if (buffer[0u] == 'S') {
                scanf("%" STR_SIZE_"s", buffer);
                bool result = PatriciaTreeSave(tree, buffer);
                if (result) {
                    printf("OK\n");
                }
            }
            else {
                scanf("%" STR_SIZE_"s", buffer);
                TPatriciaTree *temp = PatriciaTreeLoad(buffer);
                if (temp != NULL) {
                    PatriciaTreeDestroy(tree);
                    tree = temp;
                    printf("OK\n");
                }
            }
        }
    }

    PatriciaTreeDestroy(tree);

    return 0;
}