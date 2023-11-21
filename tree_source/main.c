#include "../tree_include/tree.h"

const char* FILE_PATH = "file.tre";

int main()
{
    FILE* file = fopen(FILE_PATH, "w");
    if (file == NULL)
    {
	    fprintf(stderr, "unable to open %s\n", FILE_PATH);
	    return -1;
    }
    Tree tree = {};
    treeCtor(&tree, file);
    tree.rootBranch->data = 10;
    tree.rootBranch-> leftBranch = treeCreateNode(&tree, NULL, NULL, NULL, 0);
    tree.rootBranch->rightBranch = treeCreateNode(&tree, NULL, NULL, NULL, 0); 
    tree.rootBranch->leftBranch->rightBranch = treeCreateNode(&tree, NULL, NULL, NULL, 0);
    tree.rootBranch->leftBranch-> leftBranch = treeCreateNode(&tree, NULL, NULL, NULL, 0);
    tree.rootBranch->leftBranch-> leftBranch->leftBranch = treeCreateNode(&tree, NULL, NULL, NULL, 0); 

    treePrintPreorder(tree.rootBranch, file);
    fputc('\n', file);

    fclose(file);

    treeVerify(&tree);
    return 0;
}
