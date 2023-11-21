#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <string.h>
#include "tree_cfg.h"
#include "tree_dump.h"

typedef struct TreeNode
{
    treeElem_t data;
    struct TreeNode* leftBranch;
    struct TreeNode* rightBranch;
    struct TreeNode* parentBranch;
} TreeNode;

typedef struct
{
    TreeNode* rootBranch;
    unsigned int size;
    unsigned int capacity;
    unsigned int freeIndex;
    TreeNode*    memBuffer;
} Tree;

typedef enum
{
    #define DEF_TREE_ERR(err, msg) TREE_ERROR_ ## err,
    #include "tree_err_codegen.inc"
    #undef DEF_TREE_ERR
} TreeError;

TreeError treeCtor(Tree* tree);

TreeError treeDtor(Tree* tree);

TreeError treeInsertLeft (Tree* tree, TreeNode* node, treeElem_t data);
TreeError treeInsertRight(Tree* tree, TreeNode* node, treeElem_t data);

void treePrintPostorder(TreeNode* node, FILE* file);
void treePrintPreorder (TreeNode* node, FILE* file);
void treePrintInorder  (TreeNode* node, FILE* file);

const char* treeGetErrorMsg(TreeError err);

TreeNode* treeCreateNode(Tree* tree, TreeNode* left, TreeNode* right,
                                     TreeNode* parent, treeElem_t data);

Tree* treeLoad(FILE* file, int direction);


#endif // TREE_H_