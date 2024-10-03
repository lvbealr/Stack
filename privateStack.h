#ifndef PRIVATE_STACK_H_
#define PRIVATE_STACK_H_

#include <cstdlib>

#include "privateStack.h"

typedef int stack_t;

struct stack;

const size_t  START_STACK_SIZE = 20;

// FUNCTION PROTOTYPES //
int        stackInitialize         (stack *stack, size_t   size);
int        stackDestruct           (stack *stack);
int        stackPush               (stack *stack, stack_t  value);
int        stackPop                (stack *stack, stack_t *variable);
int        destructPrivateStack    (stack *stack);
int        setDumpFileName         (stack *stack);
stack     *initializePrivateStack  (const char *fileName, const int line, const char *function);
// FUNCTION PROTOTYPES //

#endif