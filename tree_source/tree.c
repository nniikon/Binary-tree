#include "../tree_include/tree.h"

#include <assert.h>
#include "../tree_include/tree_fileOperations.h"


#define TREE_DUMP_RETURN_ERROR(err)                                           \
    do                                                                        \
    {                                                                         \
        if (tree->debugInfo.isDebug)                                          \
            fprintf(tree->debugInfo.file, "%s\n", treeGetErrorMsg(err));      \
        return err;                                                           \
    } while (0)


#define TREE_VERIFY_OR_ELSE(tree)                                             \
    do                                                                        \
    {                                                                         \
        TreeError err = treeVerify(tree);                                     \
        if (err != TREE_ERROR_NO)                                             \
            TREE_DUMP_RETURN_ERROR(err);                                      \
    } while (0)


typedef enum TreePrintOrder
{
    TREE_ORDER_INORDER,
    TREE_ORDER_PREORDER,
    TREE_ORDER_POSTORDER,
} TreePrintOrder;


typedef enum TreeDirection
{
    TREE_DIR_RIGHT,
    TREE_DIR_LEFT,
} TreeDirection;


static TreeNode* treeCalloc(Tree* tree);

static TreeError treeInsert(Tree* tree, TreeNode* node, TreeDirection dir, treeElem_t data);

static void treePrint(TreeNode* node, FILE* file, TreePrintOrder order);

static unsigned int treeCalcSize(Tree* tree);

static unsigned int treeCalcSize_recursive(TreeNode* node);


const char* treeGetErrorMsg(TreeError err)
{
    #define DEF_TREE_ERR(err, msg)                                            \
        case TREE_ERROR_ ## err:                                              \
            return msg;                                                       \

    switch (err)
    {
        #include "../tree_include/tree_err_codegen.inc"
        default:
            return "No such error was found";
    }
    #undef DEF_TREE_ERR
}


static TreeNode* treeCalloc(Tree* tree)
{
    TREE_DUMP_FUNC_START(tree->debugInfo.file);

    if (tree == NULL) 
        return NULL;
    if (tree->capacity < tree->freeIndex)
        return NULL;

    if (tree->freeIndex == tree->capacity)
    {
        unsigned int newCapacity = 0;

        if (tree->capacity == 0)
            newCapacity = TREE_DEFAULT_CAPACITY;
        else
            newCapacity = tree->capacity * TREE_CAPACITY_GROWTH_FACTOR;

        tree->memBuffer = realloc(tree->memBuffer,
                    sizeof(tree->memBuffer[0]) * newCapacity);

        if (tree->memBuffer == NULL)
            return NULL;

        memset(tree->memBuffer + tree->freeIndex, 0,
                    sizeof(tree->memBuffer[0]) * (newCapacity - tree->capacity));
        tree->capacity = newCapacity;
    }

    TREE_DUMP_FUNC_SUCCESS(tree->debugInfo.file);
    return tree->memBuffer + tree->freeIndex++;
}


TreeError treeCtor(Tree* tree, FILE* file)
{
	if (tree == NULL)
		return TREE_ERROR_NULLPTR_PASSED;
	memset(tree, 0, sizeof(Tree));

	if (file != NULL)
	{
		tree->debugInfo.isDebug = 1;
		tree->debugInfo.file = file;
		tree->debugInfo.dumpIndex = 0;
	}
	else
	{
		tree->debugInfo.isDebug = 0;
		tree->debugInfo.file = NULL;
		tree->debugInfo.dumpIndex = (unsigned int) -1;
	}

    tree->rootBranch = treeCalloc(tree);
    if (tree->rootBranch == NULL)
    {
        TREE_DUMP_RETURN_ERROR(TREE_ERROR_BAD_MEM_ALLOC);
    }
    tree->size++;



    TREE_DUMP_FUNC_SUCCESS(tree->debugInfo.file);
    return TREE_ERROR_NO;
}


TreeError treeDtor(Tree* tree)
{
    TREE_DUMP_FUNC_START(tree->debugInfo.file);

    free(tree->memBuffer);
    memset(tree, 0, sizeof(Tree));

    TREE_DUMP_FUNC_SUCCESS(tree->debugInfo.file);
    return TREE_ERROR_NO;
}


TreeError treeVerify(Tree* tree)
{
    if (tree == NULL)
        TREE_DUMP_RETURN_ERROR(TREE_ERROR_NULLPTR_PASSED);

    if (tree->rootBranch == NULL || tree->memBuffer == NULL)
        TREE_DUMP_RETURN_ERROR(TREE_ERROR_NO_CTOR);

    if (tree->size > tree->capacity)
        TREE_DUMP_RETURN_ERROR(TREE_ERROR_SIZE_CAPACITY);

    if (tree->size != treeCalcSize(tree))
        TREE_DUMP_RETURN_ERROR(TREE_ERROR_UNEXPECTED_SIZE);

    return TREE_ERROR_NO;
}


static unsigned int treeCalcSize_recursive(TreeNode* node)
{
    unsigned int addSize = 1;

    if (node->rightBranch != NULL)
        addSize += treeCalcSize_recursive(node->rightBranch);
    if (node-> leftBranch != NULL)
        addSize += treeCalcSize_recursive(node-> leftBranch);

    return addSize;
}


static unsigned int treeCalcSize(Tree* tree)
{
    return treeCalcSize_recursive(tree->rootBranch);
}


static TreeError treeInsert(Tree* tree, TreeNode* node, TreeDirection dir, treeElem_t data)
{
    TREE_VERIFY_OR_ELSE(tree);
    if (node == NULL)
        TREE_DUMP_RETURN_ERROR(TREE_ERROR_NULLPTR_PASSED);

    TreeNode* newNode = treeCalloc(tree);
    if (newNode == NULL)
        TREE_DUMP_RETURN_ERROR(TREE_ERROR_BAD_MEM_ALLOC);
    newNode->data = data;
    tree->size++;

    switch(dir)
    {
        case TREE_DIR_RIGHT:
            node->rightBranch = newNode;
            break;
        case TREE_DIR_LEFT:
            node->leftBranch  = newNode;
            break;
        default:
            assert(0);
            return TREE_ERROR_UNEXPECTED;
    }
    return TREE_ERROR_NO;
}


TreeError treeInsertRight(Tree* tree, TreeNode* node, treeElem_t data)
{
    return treeInsert(tree, node, TREE_DIR_RIGHT, data);
}


TreeError treeInsertLeft(Tree* tree, TreeNode* node, treeElem_t data)
{
    return treeInsert(tree, node, TREE_DIR_LEFT , data);
}


static void treePrint(TreeNode* node, FILE* file, TreePrintOrder order)
{
    if (node == NULL)
    {
        fprintf(file, "nil ");
        return;
    }
    fprintf(file, "( ");

    if (order ==  TREE_ORDER_PREORDER)
        fprintf(file, TREE_ELEM_FORMAT " " ,node->data);

    treePrint(node->leftBranch, file, order);

    if (order == TREE_ORDER_INORDER)
        fprintf(file, TREE_ELEM_FORMAT " " ,node->data);

    treePrint(node->rightBranch, file, order);

    if (order == TREE_ORDER_POSTORDER)
        fprintf(file, TREE_ELEM_FORMAT " " ,node->data);

    fprintf(file, ") ");
}


void treePrintPostorder(TreeNode* node, FILE* file)
{
    treePrint(node, file, TREE_ORDER_POSTORDER);
}


void treePrintPreorder(TreeNode* node, FILE* file)
{
    treePrint(node, file, TREE_ORDER_PREORDER);
}


void treePrintInorder(TreeNode* node, FILE* file)
{
    treePrint(node, file, TREE_ORDER_INORDER);
}


// Tree* treeLoad(FILE* file, int direction)
// {
//     TREE_DUMP_FUNC_START;

//     Tree tree = {};
//     treeCtor(&tree);

//     size_t size;
//     char* buffer = NULL;
//     createCharBuffer(&buffer, &size, file);

//     free(buffer);
//     TREE_DUMP_FUNC_SUCCESS;
//     return TREE_ERROR_NO;
// }


// static TreeNode* treeLoadPreorder(Tree* tree, char* buffer)
// {
//     const char* delims = ")\n";
//     TreeNode* node = treeCalloc(tree);
//     if (node == NULL)
//         return node;

//     while (strchr(delims, buffer) == NULL)
//     {
//         if (buffer[0] == '(')
//         {
//             buffer += 2;
//             node->leftBranch = treeLoadPreorder(tree, buffer);
//         }
//         else if (isdigit(buffer[0]))
//         {
//             node->data = atof
//         }

//     }
// }


TreeNode* treeCreateNode(Tree* tree, TreeNode* left, TreeNode* right, TreeNode* parent, treeElem_t data)
{
    TreeNode* node = treeCalloc(tree);
    if (node == NULL)
        return NULL;

    tree->size++;

    node->data         = data;
    node->leftBranch   = left;
    node->rightBranch  = right;
    node->parentBranch = parent;

    return node;
}

TreeNode* treeCreateEmptyNode(Tree* tree)
{
    TreeNode* node = treeCalloc(tree);
    if (node != NULL)
        tree->size++;
    return node;
}
