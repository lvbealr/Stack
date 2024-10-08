#ifndef CUSTOM_EXITS_H_
#define CUSTOM_EXITS_H_

#include <cstdio>
#include <cstdlib>

#define customWarning(expression, returnMessage) do {                               \
    if (!(expression)) {                                                            \
        printf("[WARNING]: %s, file (%s), function (%s), line %d [%s:%d]\n",        \
                #expression, __FILE__, __FUNCTION__, __LINE__, __FILE__, __LINE__); \
        STACK->errorStatus |= 1 << returnMessage;                                   \
        printf("[ERROR BINARY CODE]: ");                                            \
        printBinaryErrorStatus(STACK->errorStatus);                                 \
        printf("\n");                                                               \
        return returnMessage;                                                       \
    }                                                                               \
} while (0)

#define customAssert(expression, returnMessage) do {                                \
    if (!(expression)) {                                                            \
        printf("[WARNING]: %s, file (%s), function (%s), line %d [%s:%d]\n",        \
                #expression, __FILE__, __FUNCTION__, __LINE__, __FILE__, __LINE__); \
        STACK->errorStatus |= 1 << returnMessage;                                   \
        printf("[ERROR BINARY CODE]: ");                                            \
        printBinaryErrorStatus(STACK->errorStatus);                                 \
        printf("\n");                                                               \
        exit(0);                                                                    \
        return returnMessage;                                                       \
    }                                                                               \
} while (0)

#endif // CUSTOM_EXITS_H_