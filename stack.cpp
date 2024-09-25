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

    stack->size     = 0;
    stack->capacity = capacity;
    stack->data     = (stack_t *)calloc(capacity, sizeof(stack_t));

    customWarning(stack->data != NULL, 1);

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

static inline int stackResize(stack *stack, const changeMemory changeMemoryMode) {
    customWarning(!stackCheck(stack), 1);

    DUMP_(stack);

    if (changeMemoryMode == ADD_MEMORY) {
        stack->data = (stack_t *)realloc(stack->data, stack->capacity * sizeof(stack) * 2);
        customWarning(stack->data != NULL, 1);
        stack->capacity *= 2;
    }
    else if (changeMemoryMode == DUMP_MEMORY) {
        stack->data = (stack_t *)realloc(stack->data, stack->capacity / sizeof(stack) / 2);
        customWarning(stack->data != NULL, 1);
        stack->capacity /= 2;
    }
    
    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}

int stackCheck(stack *stack) {    
    customWarning(stack           != NULL,            1);
    customWarning(stack->capacity >= 0,               1);
    customWarning(stack->size     >= 0,               1);
    customWarning(stack->size     <= stack->capacity, 1);

    return 0;
}

