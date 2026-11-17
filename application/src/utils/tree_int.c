/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "tree_int.h"

TreeInt* TreeInt_create(void)
{
    TreeInt* tree = (TreeInt*)calloc(1, sizeof(TreeInt));
    AssertNew(tree);
    return tree;
}

TreeIntNode* TreeInt_nodeCreate(int value)
{
    TreeIntNode* node = (TreeIntNode*)calloc(1, sizeof(TreeIntNode));
    AssertNew(node);
    node->value = value;
    return node;
}

void TreeInt_destroyRec(TreeIntNode* node)
{
    if (!node) return;
    TreeInt_destroyRec(node->firstChild);
    TreeInt_destroyRec(node->nextSibling);
    free(node);
}

void TreeInt_destroy(TreeInt* tree)
{
    if (!tree) return;
    TreeInt_destroyRec(tree->root);
    free(tree);
}

void TreeInt_addChild(TreeIntNode* node, TreeIntNode* child)
{
    if ((!node) || (!child)) return;
    //if (node == NULL) // Ajout à la racine
    //{
    //    if (tree->root != NULL) // Si racine déjà présente
    //        child->nextSibling = tree->root->firstChild;

    //    tree->root = child;
    //}
    //else
    //{
    child->nextSibling = node->firstChild;
    node->firstChild = child;
    //}
}

TreeIntNode* TreeInt_getRec(TreeIntNode* node, int value)
{
    if ((!node) || (node->value == value)) return node;

    TreeIntNode* res = TreeInt_getRec(node->firstChild, value);
    if (res) return res;

    res = TreeInt_getRec(node->nextSibling, value);
    if (res) return res;

    return NULL;
}

TreeIntNode* TreeInt_get(TreeInt* tree, int value)
{
    assert(tree);
    return TreeInt_getRec(tree->root, value);
}

static int TreeInt_numberLen(int value)
{
    int res = 1;
    while (value >= 10)
    {
        value /= 10;
        res++;
    }
    return res;
}

static int TreeInt_maxNumberLen(TreeIntNode* node)
{
    if (!node) return 0;
    int score = TreeInt_numberLen(node->value);
    int score_ = TreeInt_maxNumberLen(node->firstChild);
    if (score < score_) score = score_;
    score_ = TreeInt_maxNumberLen(node->nextSibling);
    if (score < score_) score = score_;
    return score;
}

#define TREE_INT_PRINT_MAX_LINE_COUNT 200
#define TREE_INT_PRINT_MAX_COLUMN_COUNT 1024

int TreeInt_printRec(TreeIntNode* node, int level, int line, char** map, int maxLen)
{
    if (!node) return 0;
    if (line >= TREE_INT_PRINT_MAX_LINE_COUNT) return 0;

    int left = TreeInt_printRec(node->firstChild, level + 1, line, map, maxLen);

    char buffer[1024] = { 0 };
    int len = TreeInt_numberLen(node->value);
    int i = 0;
    for (i = 0; i < maxLen - len; i++) buffer[i] = '-';

    if ((node->firstChild) && (node->firstChild->nextSibling))
    {
        sprintf(buffer + i, "[%d]+", node->value);
    }
    else
    {
        sprintf(buffer + i, "[%d] ", node->value);
    }

    for (int i = 0; i < maxLen + 3; i++)
        map[line][level * (maxLen + 3) + i] = buffer[i];

    int lineCount = (left > 0) ? left : 1;
    int line_ = line;
    if ((node->firstChild == NULL) || (node->firstChild->nextSibling == NULL))
    {
        int column = level * (maxLen + 3) + maxLen + 2;
        if (node->firstChild && column < TREE_INT_PRINT_MAX_COLUMN_COUNT)
            map[line][column] = '-';

        return lineCount;
    }
    TreeIntNode* current = node->firstChild->nextSibling;
    while (current)
    {
        int column = level * (maxLen + 3) + maxLen + 2;
        if (column < TREE_INT_PRINT_MAX_COLUMN_COUNT)
        {
            for (int j = line_ + 1; j <= line + lineCount; j++)
            {
                if (j >= TREE_INT_PRINT_MAX_LINE_COUNT) return lineCount;
                map[j][column] = '|';
            }
            if (line + lineCount + 1 >= TREE_INT_PRINT_MAX_LINE_COUNT) return lineCount;
            map[line + lineCount + 1][column] = '+';
        }
        line_ = line + lineCount + 1;
        lineCount += TreeInt_printRec(current, level + 1, line + lineCount + 1, map, maxLen) + 1;
        current = current->nextSibling;
    }
    return lineCount;
}

void TreeInt_print(TreeInt* tree)
{
    char** map = (char**)calloc(TREE_INT_PRINT_MAX_LINE_COUNT, sizeof(char*));
    AssertNew(map);
    for (int i = 0; i < TREE_INT_PRINT_MAX_LINE_COUNT; i++)
    {
        map[i] = (char*)calloc(TREE_INT_PRINT_MAX_COLUMN_COUNT + 1, sizeof(char));
        AssertNew(map[i]);
        for (int j = 0; j < TREE_INT_PRINT_MAX_COLUMN_COUNT; j++) map[i][j] = ' ';
    }
    int maxLen = TreeInt_maxNumberLen(tree->root);
    TreeInt_printRec(tree->root, 0, 0, map, maxLen);

    for (int i = 0; i < TREE_INT_PRINT_MAX_LINE_COUNT; i++)
    {
        bool end = true;
        for (int j = 0; j < TREE_INT_PRINT_MAX_COLUMN_COUNT; j++)
        {
            if (map[i][j] != ' ')
            {
                end = false;
                break;
            }
        }
        if (end) break;

        int lastCar = 0;
        for (int j = 0; j < TREE_INT_PRINT_MAX_COLUMN_COUNT; j++)
        {
            if (map[i][j] != ' ')
            {
                lastCar = j;
            }
        }
        map[i][lastCar + 1] = '\0';
        printf("%s\n", map[i]);
    }

    for (int i = 0; i < TREE_INT_PRINT_MAX_LINE_COUNT; i++)
    {
        free(map[i]);
    }
    free(map);
}
