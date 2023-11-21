#include "../tree_include/tree.h"


const char* FILE_PATH = "file.tre";

int main()
{
    FILE* file = fopen(FILE_PATH, "w");
    Tree tree;
    treeCtor(&tree);
    tree.rootBranch->data = 10;
    treeInsertRight(&tree, tree.rootBranch, 15);
    treeInsertRight(&tree, tree.rootBranch->rightBranch, 20);
    treeInsertLeft (&tree, tree.rootBranch->rightBranch, 12);
    treeInsertLeft (&tree, tree.rootBranch, 5);
    treePrintPostorder(tree.rootBranch, file);
    fputc('\n', file);
    treePrintPreorder(tree.rootBranch, file);
    fputc('\n', file);
    treePrintInorder(tree.rootBranch, file); 
    fputc('\n', file);
    fclose(file);

    file = fopen(FILE_PATH, "r");
    treeLoad(file, 0);
    fclose(file);
    return 0;
}
