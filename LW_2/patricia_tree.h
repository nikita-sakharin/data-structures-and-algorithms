#ifndef _PATRICIA_TREE_H_
#define _PATRICIA_TREE_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct patriciaTree TPatriciaTree;

TPatriciaTree *PatriciaTreeCreate(size_t size);

void PatriciaTreeDestroy(TPatriciaTree *tree);

void PatriciaTreeClear(TPatriciaTree *tree);

bool PatriciaTreeSave(TPatriciaTree *tree, const char *file_name);

TPatriciaTree *PatriciaTreeLoad(const char *file_name);

size_t PatriciaTreeSizeof(const TPatriciaTree *tree);

bool PatriciaTreeEmpty(const TPatriciaTree *tree);

size_t PatriciaTreeSize(const TPatriciaTree *tree);

bool PatriciaTreeInsert(TPatriciaTree *tree, const char *key, size_t key_size, const void *value);

bool PatriciaTreeFind(const TPatriciaTree *tree, const char *key, size_t key_size, void *value);

bool PatriciaTreeDelete(TPatriciaTree *tree, const char *key, size_t key_size);
/*
void PatriciaTreePrint(const TPatriciaTree *tree);
*/
#endif