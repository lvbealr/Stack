#include <cstdio>
#include <cstdlib>

#include "stackDump.h"
#include "stack.h"
#include "customWarning/customWarning.h"

// WHAT BREAK BY WARNING
// WHAT BREAK BY ASSERT 
// SOLVE THIS ??????????

int stackInitialize(stack *stack, int capacity) {
    customWarning(!stackCheck(stack), 1);

    printf("ВНУТРИ stackInitialize: %s\n", stack->dumpFile);
    stack->size     = 0;
    stack->capacity = capacity;
    stack->data     = (stack_t *)calloc(capacity, sizeof(stack_t));

    // TODO check for null?

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}

int stackDestruct(stack *stack) {
    customWarning(!stackCheck(stack), 1);

    stack->size     = 0;
    stack->capacity = 0;

    free(stack->data);

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}


int stackPush(stack *stack, stack_t value) {
    customWarning(!stackCheck(stack), 1);
    customWarning(value != BAD_VALUE, 1);

    if (stack->size >= stack->capacity) {
        stackResize(stack, ADD_MEMORY);
    }

    stack->data[stack->size++] = value;

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}


int stackPop(stack *stack, stack_t *variable) {
    customWarning(!stackCheck(stack),  1);
    customWarning(stack->size >  0,    1);
    customWarning(variable    != NULL, 1);

    if (stack->size <= stack->capacity / 4) {
        stackResize(stack, DUMP_MEMORY);
    }

    *variable = stack->data[--stack->size];

    DUMP_(stack);

    customWarning(!stackCheck(stack),  1);

    return 0;
    
}

static inline int stackResize(stack *stack, const int mode) {
    customWarning(!stackCheck(stack), 1);

    DUMP_(stack);

    // TODO what the fuck is mode?
    if (mode == 1) {
        stack->data = (stack_t *)realloc(stack->data, stack->capacity * sizeof(stack) * 2);
        stack->capacity *= 2;
    }
    else if (mode == 0) {
        stack->data = (stack_t *)realloc(stack->data, stack->capacity / sizeof(stack) / 2);
        stack->capacity /= 2;
    }
    
    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}

int stackCheck(stack *stack) {    
    customWarning(stack           != NULL,            1);
    // customWarning(stack->data     != NULL,            1);
    customWarning(stack->capacity >= 0,               1);
    customWarning(stack->size     >= 0,               1);
    customWarning(stack->size     <= stack->capacity, 1);

    return 0;
}

