#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "stackDump.h"
#include "stack.h"
#include "customWarning/customWarning.h"
#include "stackHash.h"

#define SPECIFICATOR_TYPE "d"

// TODO WHAT BREAK BY WARNING
// TODO WHAT BREAK BY ASSERT 
// TODO SOLVE THIS

// TODO ALL POINTERS TO NULL AFTER FREE!!!!!!
// TODO UINT64_T IN 2 MEMORY CELLS FOR STACK_T (INT) || USE MEMORY CAST

// TODO ANYWHERE HASH_CHECK

int stackInitialize(stack *stack, size_t capacity) {
    customWarning(!stackCheck(stack), 1);

    stack->size         = 0;
    stack->capacity     = capacity;
    stack->memoryChunk  = (stack_t *)calloc(capacity + COUNT_OF_CANARIES, sizeof(stack_t));
    stack->data         = stack->memoryChunk + CANARY_SHIFT;
    stack->hash         = hashValue;

    DATA_BEGIN_CANARY_INITIALIZE(stack->memoryChunk);
    DATA_END_CANARY_INITIALIZE  (stack->memoryChunk, stack->capacity + CANARY_SHIFT);

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

    printf("CALL STACK_HASH IN STACK_PUSH: %lu\n", djb2Hash(stack));

    printf("BEFORE IN STACK_PUSH: %lu\n", stack->hash);

    customWarning(!djb2HashCheck(stack), 1);

    if (stack->size >= stack->capacity) {
        stackResize(stack, ADD_MEMORY);
    }

    stack->data[stack->size++] = value;

    stack->hash = djb2Hash(stack);

    printf("AFTER IN STACK_PUSH: %lu\n", stack->hash);

    customWarning(!djb2HashCheck(stack), 1);

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}


int stackPop(stack *stack, stack_t *variable) {
    customWarning(!stackCheck(stack),  1);
    customWarning(stack->size >  0,    1);
    customWarning(variable    != NULL, 1);

    printf("CALL STACK_HASH IN STACK_POP: %lu\n", djb2Hash(stack));

    printf("BEFORE IN STACK_POP: %lu\n", stack->hash);

    customWarning(!djb2HashCheck(stack), 1);

    if (stack->size <= stack->capacity / 4) {
        stackResize(stack, DUMP_MEMORY);
    }

    *variable                = stack->data[--stack->size];
    stack->data[stack->size] = POISON_VALUE;

    stack->hash = djb2Hash(stack);

    printf("AFTER IN STACK_POP: %lu\n", stack->hash);

    customWarning(!djb2HashCheck(stack), 1);

    DUMP_(stack);

    customWarning(!stackCheck(stack),  1);

    return 0;
    
}

static int stackResize(stack *stack, const changeMemory changeMemoryMode) {
    customWarning(!stackCheck(stack), 1);

    DUMP_(stack);

    if (changeMemoryMode == ADD_MEMORY) {
        size_t newCapacity = stack->capacity * 2;
        
        stack->memoryChunk = (stack_t *)realloc(stack->memoryChunk,
                                                sizeof(stack_t) * (newCapacity + COUNT_OF_CANARIES));

        customWarning(stack->memoryChunk != NULL, 1);

        DATA_BEGIN_CANARY_INITIALIZE(stack->memoryChunk);
        DATA_END_CANARY_INITIALIZE  (stack->memoryChunk, newCapacity + CANARY_SHIFT);

        stack->data         = stack->memoryChunk + CANARY_SHIFT;
        stack->capacity    *= 2;

        stackFillPoison(stack); // TODO customRealloc (realloc + fill)
    }

    else if (changeMemoryMode == DUMP_MEMORY) {
        size_t newCapacity = stack->capacity / 2;

        stack->memoryChunk = (stack_t *)realloc(stack->memoryChunk,
                                         sizeof(stack_t) * (newCapacity + COUNT_OF_CANARIES));

        customWarning(stack->memoryChunk != NULL, 1);

        DATA_BEGIN_CANARY_INITIALIZE(stack->memoryChunk);
        DATA_END_CANARY_INITIALIZE  (stack->memoryChunk, newCapacity + CANARY_SHIFT);

        stack->data         = stack->memoryChunk + CANARY_SHIFT;
        stack->capacity    /= 2;

        stackFillPoison(stack); // TODO customRealloc (realloc + fill)
    }

    DUMP_(stack);

    customWarning(!stackCheck(stack), 1);

    return 0;
}

// TODO MORE CHECKERS
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