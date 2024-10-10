#ifndef CUSTOM_EXITS_H_
#define CUSTOM_EXITS_H_

#include <cstdio>
#include <cstdlib>

#include "stack.h"
#include "stackDump.h"

extern stack *STACK_PTR;

#define customWarning(expression, returnMessage) do {                               \
    if (!(expression)) {                                                            \
        printf("[WARNING]: %s, file (%s), function (%s), line %d [%s:%d]\n",        \
                #expression, __FILE__, __FUNCTION__, __LINE__, __FILE__, __LINE__); \
        STACK_PTR->errorStatus |= returnMessage;                                    \
        return returnMessage;                                                       \
    }                                                                               \
} while (0)

#define customAssert(expression, returnMessage) do {                                \
    if (!(expression)) {                                                            \
        printf("[WARNING]: %s, file (%s), function (%s), line %d [%s:%d]\n",        \
                #expression, __FILE__, __FUNCTION__, __LINE__, __FILE__, __LINE__); \
        STACK_PTR->errorStatus |= returnMessage;                                    \
        printf("[ERROR BINARY CODE]: ");                                            \
        printBinaryErrorStatus(STACK_PTR->errorStatus);                             \
        printf("\n");                                                               \
        printErrorDescription(STACK_PTR->errorStatus);                              \
        DUMP_(STACK);                                                               \
        exit(0);                                                                    \
        return returnMessage;                                                       \
    }                                                                               \
} while (0)

#endif // CUSTOM_EXITS_H_