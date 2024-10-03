#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "stackDump.h"
#include "stack.h"
#include "customWarning/customWarning.h"

#define SPECIFICATOR_TYPE "d"

// TODO WHAT BREAK BY WARNING
// TODO WHAT BREAK BY ASSERT 
// TODO SOLVE THIS

// TODO ALL POINTERS TO NULL AFTER FREE!!!!!!
// TODO UINT64_T IN 2 MEMORY CELLS FOR STACK_T (INT) ???? use address

int stackInitialize(stack *stack, size_t capacity) {
    customWarning(!stackCheck(stack), 1);

    stack->size         = 0;
    stack->capacity     = capacity;
    stack->memoryChunk  = (stack_t *)calloc(capacity + 2 * CANARY_SIZE(CANARY), sizeof(stack_t));
    stack->data         = stack->memoryChunk + CANARY_SIZE(CANARY);

    DATA_BEGIN_CANARY_INITIALIZE(stack->memoryChunk);
    DATA_END_CANARY_INITIALIZE  (stack->memoryChunk, stack->capacity + CANARY_SIZE(CANARY));

    customWarning(stack->data != NULL, 1);

    stackFillPoison(stack);

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}

int stackFillPoison(stack *stack) {
    customWarning(!stackCheck(stack), 1);

    for (size_t index = stack->size; index < stack->capacity; index++) {
        stack->data[index] = POISON_VALUE;
    }

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}

int stackDestruct(stack *stack) {
    customWarning(!stackCheck(stack), 1);

    stack->size     = 0;
    stack->capacity = 0;

    free(stack->memoryChunk);
    stack->memoryChunk = NULL;

    CANARY_DESTRUCT();

    DUMP_(stack);

    free(stack->dumpFile);
    stack->dumpFile = NULL;

    customWarning(!stackCheck(stack), 1);

    return 0;
}


int stackPush(stack *stack, stack_t value) {
    customWarning(!stackCheck(stack), 1);
    customWarning(value != POISON_VALUE, 1);

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

    *variable                = stack->data[--stack->size];
    stack->data[stack->size] = POISON_VALUE;

    DUMP_(stack);

    customWarning(!stackCheck(stack),  1);

    return 0;
    
}

static int stackResize(stack *stack, const changeMemory changeMemoryMode) {
    customWarning(!stackCheck(stack), 1);

    DUMP_(stack);

    if (changeMemoryMode == ADD_MEMORY) {
        stack->memoryChunk = (stack_t *)realloc(stack->memoryChunk,
                                                sizeof(stack_t) * (stack->capacity * 2 + CANARY_SIZE(CANARY) * 2));

        customWarning(stack->memoryChunk != NULL, 1);

        DATA_BEGIN_CANARY_INITIALIZE(stack->memoryChunk);
        DATA_END_CANARY_INITIALIZE  (stack->memoryChunk, stack->capacity * 2 + CANARY_SIZE(CANARY));

        stack->data         = stack->memoryChunk + CANARY_SIZE(CANARY);
        stack->capacity    *= 2;

        stackFillPoison(stack); // TODO customRealloc (realloc + fill)
    }

    else if (changeMemoryMode == DUMP_MEMORY) {
        stack->memoryChunk = (stack_t *)realloc(stack->memoryChunk,
                                         sizeof(stack_t) * (stack->capacity / 2 + CANARY_SIZE(CANARY) * 2));

        customWarning(stack->memoryChunk != NULL, 1);

        DATA_BEGIN_CANARY_INITIALIZE(stack->memoryChunk);
        DATA_END_CANARY_INITIALIZE  (stack->memoryChunk, stack->capacity / 2 + CANARY_SIZE(CANARY));

        stack->data         = stack->memoryChunk + CANARY_SIZE(CANARY);
        stack->capacity    /= 2;

        stackFillPoison(stack); // TODO customRealloc (realloc + fill)
    }

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}

int stackCheck(stack *stack) {
    customWarning(stack              != NULL,            1);
    // customWarning(stack->memoryChunk != NULL,            1);

    if (stack->memoryChunk != NULL) {
        CHECK_CANARY();
    }
    customWarning(stack->size        <= stack->capacity, 1);

    return 0;
}

int printStack(stack *stack) {
    customWarning(!stackCheck(stack), 1);

    for (size_t index = 0; index < stack->capacity; index++) {
        printf("[%lu, %p] = %" SPECIFICATOR_TYPE "\n", index, stack->data + index, stack->data[index]);
    }

    return 0;
}