#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include <stdio.h>

#ifndef TREE_RELEASE
    #define TREE_DUMP(file ,...)                                            \
    do                                                                      \
    {                                                                       \
        if (file != NULL)                                                   \
        {                                                                   \
            fprintf(file, "%s[%d]\t\t", __FILE__, __LINE__);                \
            fprintf(file, __VA_ARGS__);                                     \
        }                                                                   \
    } while (0)
    #define TREE_DUMP_FUNC_START(file)   TREE_DUMP(file, "%s started\n", __PRETTY_FUNCTION__)
    #define TREE_DUMP_FUNC_SUCCESS(file) TREE_DUMP(file, "%s success\n", __PRETTY_FUNCTION__)
#else
    #define TREE_DUMP              do {} while (0)
    #define TREE_DUMP_FUNC_START   do {} while (0)
    #define TREE_DUMP_FUNC_SUCCESS do {} while (0)
#endif

#endif // TREE_DUMP_H
