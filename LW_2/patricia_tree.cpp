#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patricia_tree.h"

typedef struct patriciaTreeNode TPatriciaTreeNode;

struct patriciaTreeNode
{
    TPatriciaTreeNode *left;
    TPatriciaTreeNode *right;
    void *value;
    char *key;
    size_t keySize;
    ptrdiff_t diffBit;
};

struct patriciaTree
{
    TPatriciaTreeNode *root;
    size_t valueSize;
    size_t count;
};

enum {
    SIGN_BIT_CHAR = 5u,
    CHAR_BIT_MASK = 31
};

static inline bool GetBit(const char *string, size_t size, ptrdiff_t bit)
{
    if (bit < 0)
    {
        return false;
    }

    if ((size_t) bit / SIGN_BIT_CHAR >= size)
    {
        return false;
    }

    return (string[bit / SIGN_BIT_CHAR] & 1 << bit % SIGN_BIT_CHAR) != 0;
}

static inline ptrdiff_t GetFirstDiffBit(register const char *str1, register const char *str2)
{
    register ptrdiff_t byte = 0;
    while (str1[byte] && str2[byte] && (str1[byte] & CHAR_BIT_MASK) == (str2[byte] & CHAR_BIT_MASK))
    {
        ++byte;
    }

    if (!str1[byte] && !str2[byte])
    {
        return -1;
    }

    register size_t bit;
    for (bit = 0u; (str1[byte] & (1 << bit)) == (str2[byte] & (1 << bit)) && bit < SIGN_BIT_CHAR; ++bit);

    return byte * SIGN_BIT_CHAR + bit;
}
/*
#define _PATRICIA_TREE_H_SAVE_
*/
TPatriciaTree *PatriciaTreeCreate(size_t size)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (!size)
    {
        return NULL;
    }
#   endif

    register TPatriciaTree *tree = (TPatriciaTree *) malloc(sizeof(TPatriciaTree));
    if (tree == NULL)
    {
        return NULL;
    }

    tree->root = NULL;
    tree->valueSize = size;
    tree->count = 0u;

    return tree;
}

static inline void PatriciaTreeNodeDestroy(register TPatriciaTreeNode *node)
{
    free(node->value);
    free(node->key);

    if (node->diffBit < node->left->diffBit)
    {
        PatriciaTreeNodeDestroy(node->left);
    }

    if (node->right != NULL && node->diffBit < node->right->diffBit)
    {
        PatriciaTreeNodeDestroy(node->right);
    }

    free(node);
}

void PatriciaTreeDestroy(register TPatriciaTree *tree)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    if (tree->root != NULL)
    {
        PatriciaTreeNodeDestroy(tree->root);
    }

    free(tree);
}

void PatriciaTreeClear(TPatriciaTree *tree)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    if (tree->root != NULL)
    {
        PatriciaTreeNodeDestroy(tree->root);
    }

    tree->root = NULL;
    tree->count = 0u;
}

static inline void PatriciaTreeNodeSave(register TPatriciaTreeNode *node, FILE *stream, size_t valueSize)
{
    fwrite(node->value, valueSize, 1u, stream);
    fwrite(&node->keySize, sizeof(size_t), 1u, stream);
    fwrite(node->key, sizeof(char), node->keySize + 1u, stream);

    if (node->diffBit < node->left->diffBit)
    {
        PatriciaTreeNodeSave(node->left, stream, valueSize);
    }

    if (node->right != NULL && node->diffBit < node->right->diffBit)
    {
        PatriciaTreeNodeSave(node->right, stream, valueSize);
    }
}

bool PatriciaTreeSave(TPatriciaTree *tree, const char *fileName)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL || fileName == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    FILE *stream = fopen(fileName, "wb");
    if (stream == NULL)
    {
        printf("ERROR: Couldn't create file.\n");
        return false;
    }

    fwrite(&tree->valueSize, sizeof(size_t), 1u, stream);
    fwrite(&tree->count, sizeof(size_t), 1u, stream);

    if (tree->root != NULL)
    {
        PatriciaTreeNodeSave(tree->root, stream, tree->valueSize);
    }

    fclose(stream);

    return true;
}

#define STR_SIZE 257u

TPatriciaTree *PatriciaTreeLoad(const char *fileName)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (fileName == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    FILE *stream = fopen(fileName, "rb");
    if (stream == NULL)
    {
        printf("ERROR: Couldn't open file.\n");
        return NULL;
    }

    size_t valueSize;
    fread(&valueSize, sizeof(size_t), 1u, stream);

    TPatriciaTree *tree = PatriciaTreeCreate(valueSize);
    if (tree == NULL)
    {
        printf("ERROR: malloc error.\n");
        exit(EXIT_SUCCESS);
    }

    size_t count;
    fread(&count, sizeof(size_t), 1u, stream);
    char key[STR_SIZE];
    void *value = malloc(valueSize);
    if (value == NULL)
    {
        printf("ERROR: malloc error.\n");
        exit(EXIT_SUCCESS);
    }

    for (register size_t i = 0u; i < count; ++i)
    {
        fread(value, valueSize, 1u, stream);
        size_t keySize;
        fread(&keySize, sizeof(size_t), 1u, stream);
        fread(key, sizeof(char), keySize + 1u, stream);
        PatriciaTreeInsert(tree, key, keySize, value);
    }

    free(value);

    return tree;
}

size_t PatriciaTreeSizeof(register const TPatriciaTree *tree)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    return tree->valueSize;
}

bool PatriciaTreeEmpty(register const TPatriciaTree *tree)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    return !tree->count;
}

size_t PatriciaTreeSize(register const TPatriciaTree *tree)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    return tree->count;
}

bool PatriciaTreeInsert(TPatriciaTree *tree, register const char *key, size_t keySize, const void *value)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL || key == NULL || value == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    if (tree->root == NULL)
    {
        register TPatriciaTreeNode *temp = (TPatriciaTreeNode *) malloc(sizeof(TPatriciaTreeNode));
        if (temp == NULL)
        {
            printf("ERROR: malloc error.\n");
            exit(EXIT_SUCCESS);
        }

        temp->left = temp;
        temp->right = NULL;
        temp->value = malloc(tree->valueSize);
        if (temp->value == NULL)
        {
            printf("ERROR: malloc error.\n");
            exit(EXIT_SUCCESS);
        }

        memcpy(temp->value, value, tree->valueSize);
        temp->key = (char *) malloc((keySize + 1u) * sizeof(char));
        if (temp->key == NULL)
        {
            printf("ERROR: malloc error.\n");
            exit(EXIT_SUCCESS);
        }

        strcpy(temp->key, key);
        temp->keySize = keySize;
        temp->diffBit = -1;
        tree->root = temp;
        ++tree->count;

        return true;
    }

    register TPatriciaTreeNode *curr = tree->root->left;
    for (register TPatriciaTreeNode *prev = tree->root; curr->diffBit > prev->diffBit;)
    {
        prev = curr;
        curr = GetBit(key, keySize, curr->diffBit) ? curr->right : curr->left;
    }

    ptrdiff_t firstDiffBit = GetFirstDiffBit(curr->key, key);
    if (firstDiffBit < 0)
    {
        return false;
    }

    TPatriciaTreeNode **edge = &tree->root->left;
    curr = tree->root->left;
    for (register TPatriciaTreeNode *prev = tree->root; curr->diffBit > prev->diffBit && curr->diffBit < firstDiffBit;)
    {
        prev = curr;
        edge = GetBit(key, keySize, curr->diffBit) ? &curr->right : &curr->left;
        curr = *edge;
    }

    register TPatriciaTreeNode *temp = (TPatriciaTreeNode *) malloc(sizeof(TPatriciaTreeNode));
    if (temp == NULL)
    {
        printf("ERROR: malloc error.\n");
        exit(EXIT_SUCCESS);
    }

    temp->left = GetBit(key, keySize, firstDiffBit) ? curr : temp;
    temp->right = GetBit(key, keySize, firstDiffBit) ? temp : curr;
    temp->value = malloc(tree->valueSize);
    if (temp->value == NULL)
    {
        printf("ERROR: malloc error.\n");
        exit(EXIT_SUCCESS);
    }

    memcpy(temp->value, value, tree->valueSize);
    temp->key = (char *) malloc((keySize + 1u) * sizeof(char));
    if (temp->key == NULL)
    {
        printf("ERROR: malloc error.\n");
        exit(EXIT_SUCCESS);
    }

    strcpy(temp->key, key);
    temp->keySize = keySize;
    temp->diffBit = firstDiffBit;
    *edge = temp;
    ++tree->count;

    return true;
}

bool PatriciaTreeFind(const TPatriciaTree *tree, register const char *key, size_t keySize, void *value)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL || key == NULL || value == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    if (tree->root == NULL)
    {
        return false;
    }

    register TPatriciaTreeNode *curr = tree->root->left;
    for (register TPatriciaTreeNode *prev = tree->root; curr->diffBit > prev->diffBit;)
    {
        prev = curr;
        curr = GetBit(key, keySize, curr->diffBit) ? curr->right : curr->left;
    }

    if (GetFirstDiffBit(curr->key, key) < 0)
    {
        memcpy(value, curr->value, tree->valueSize);
        return true;
    }

    return false;
}

bool PatriciaTreeDelete(TPatriciaTree *tree, register const char *key, size_t keySize)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL || key == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    if (tree->root == NULL)
    {
        return false;
    }

    if (tree->root == tree->root->left)
    {
        if (GetFirstDiffBit(tree->root->key, key) >= 0)
        {
            return false;
        }

        free(tree->root->value);
        free(tree->root->key);
        free(tree->root);
        tree->root = NULL;
        --tree->count;

        return true;
    }

    register TPatriciaTreeNode *deleteNode = tree->root->left, *upToDelete = tree->root, *upParent;
    do
    {
        upParent = upToDelete;
        upToDelete = deleteNode;
        deleteNode = GetBit(key, keySize, deleteNode->diffBit) ? deleteNode->right : deleteNode->left;
    } while (deleteNode->diffBit > upToDelete->diffBit);

    if (GetFirstDiffBit(deleteNode->key, key) >= 0)
    {
        return false;
    }

    TPatriciaTreeNode *upToUp, *temp = upToDelete;
    do
    {
        upToUp = temp;
        temp = GetBit(upToDelete->key, upToDelete->keySize, temp->diffBit) ? temp->right : temp->left;
    } while (temp->diffBit > upToUp->diffBit);

    free(deleteNode->value);
    free(deleteNode->key);
    deleteNode->value = upToDelete->value;
    deleteNode->key = upToDelete->key;
    deleteNode->keySize = upToDelete->keySize;

    if (upToUp->left == upToDelete)
    {
        upToUp->left = deleteNode;
    }
    else
    {
        upToUp->right = deleteNode;
    }
    temp = upToDelete->left == deleteNode ? upToDelete->right : upToDelete->left;
    if (upParent->left == upToDelete)
    {
        upParent->left = temp;
    }
    else
    {
        upParent->right = temp;
    }

    free(upToDelete);
    --tree->count;

    return true;
}
/*
void PatriciaTreePrint(const TPatriciaTreeNode *node)
{
    printf("this = %p\n", (void *) node);
    if (node == NULL)
    {
        return;
    }

    printf("left = %p = \"%s\", right = %p = \"%s\"\n", (void *) node->left, node->left->key, (void *) node->right, node->right == NULL ? "NULL" : node->right->key);
    printf("key = \"%s\", keySize = %zu\n", node->key, node->keySize);
    printf("value = %llu = '%c'\n", *(unsigned long long *) node->value, (char) *(unsigned long long *) node->value);
    printf("diffBit = %d\n", node->diffBit);
    putchar('\n');

    if (node->left && node->diffBit < node->left->diffBit)
    {
        PatriciaTreeNodePrint(node->left);
    }

    if (node->right != NULL && node->diffBit < node->right->diffBit)
    {
        PatriciaTreeNodePrint(node->right);
    }
}

void PatriciaTreePrint(const TPatriciaTree *tree)
{
#   ifdef _PATRICIA_TREE_H_SAVE_
    if (tree == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    printf("this = %p, root = %p, valueSize = %zu, count = %zu\n", (void *) tree, (void *) tree->root, tree->valueSize, tree->count);
    putchar('\n');
    PatriciaTreeNodePrint(tree->root);
}
*/