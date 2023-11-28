#include "../include/memAllocations.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    DynArr intArr = {};
    if (dynArrCtor(&intArr, sizeof(int)) == -1)
        fprintf(stderr, "fuck\n");
    for (int i = 0; i < 4097; i++)
    {
        int* help = (int*) dynArrCalloc(&intArr);
        if (help == NULL)
        {
            fprintf(stderr, "fail!\n");
        }
    }
    dynArrDtor(&intArr);
}