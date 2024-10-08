#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "stackDump.h"
#include "privateStack.h"
#include "stack.h"
#include "customExits.h"
#include "stackHash.h"

#define SPECIFICATOR_TYPE "d"

static stackError stackResize(stack *stack, const changeMemory changeMemoryMode);

///////////////////////////////////////////////////////////
// stackError errorCode = stackCheck(stack);             //
// customWarning(errorCode == STACK_NO_ERROR, errorCode);//
///////////////////////////////////////////////////////////

stackError stackInitialize(stack *STACK, int capacity) {
    customWarning(STACK    != NULL, STACK_NULL_POINTER);
    customWarning(capacity >= 0,    STACK_BAD_CAPACITY);

    STACK->size         = 0;
    STACK->capacity     = capacity;
    STACK->memoryChunk  = (stack_t *)calloc((size_t)capacity + COUNT_OF_CANARIES, sizeof(stack_t));
    customAssert(STACK->memoryChunk != NULL, STACK_DATA_NULL_POINTER);
    
    STACK->data         = STACK->memoryChunk + CANARY_SHIFT;
    #ifndef _NDEBUG
    STACK->hash         = hashValue;
    #endif

    DATA_BEGIN_CANARY_INITIALIZE(STACK->memoryChunk);
    DATA_END_CANARY_INITIALIZE  (STACK->memoryChunk, STACK->capacity + CANARY_SHIFT);

    customAssert(STACK->data != NULL, STACK_DATA_NULL_POINTER);

    stackFillPoison(STACK);

    DUMP_(STACK);

    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
}

stackError stackFillPoison(stack *STACK) {
    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    for (int index = STACK->size; index < STACK->capacity; index++) {
        STACK->data[index] = POISON_VALUE;
    }

    DUMP_(STACK);

    errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
}

stackError stackDestruct(stack *STACK) {
    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    STACK->size     = 0;
    STACK->capacity = 0;

    free(STACK->memoryChunk);
    STACK->memoryChunk = NULL;

    CANARY_DESTRUCT();

    DUMP_(STACK);

    free(STACK->dumpFile);
    STACK->dumpFile = NULL;

    return STACK_NO_ERROR;
}


stackError stackPush(stack *STACK, stack_t value) {
    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);
    customWarning(value != POISON_VALUE, INVALID_INPUT_VALUE);

    #ifndef _NDEBUG
    customWarning(!djb2HashCheck(STACK), STACK_BAD_HASH);
    #endif

    if (STACK->size >= STACK->capacity) {
        stackResize(STACK, ADD_MEMORY);
    }

    STACK->data[STACK->size++] = value;

    #ifndef _NDEBUG
    STACK->hash = djb2Hash(STACK);
    customWarning(!djb2HashCheck(STACK), STACK_BAD_HASH);
    #endif

    DUMP_(STACK);

    #ifndef _NDEBUG
    errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);
    #endif

    return STACK_NO_ERROR;
}


stackError stackPop(stack *STACK, stack_t *variable) {
    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    customWarning(STACK->size >  0,    STACK_BAD_SIZE);
    customWarning(variable    != NULL, POP_VARIABLE_NULL_POINTER);

    #ifndef _NDEBUG
    customWarning(!djb2HashCheck(STACK), STACK_BAD_HASH);
    #endif

    if (STACK->size <= STACK->capacity / 4) {
        stackResize(STACK, DUMP_MEMORY);
    }

    *variable                = STACK->data[--STACK->size];
    STACK->data[STACK->size] = POISON_VALUE;

    #ifndef _NDEBUG
    STACK->hash = djb2Hash(STACK);
    customWarning(!djb2HashCheck(STACK), STACK_BAD_HASH);
    #endif

    DUMP_(STACK);

    errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
    
}

static stackError stackResize(stack *STACK, const changeMemory changeMemoryMode) {
    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    DUMP_(STACK);

    if (changeMemoryMode == ADD_MEMORY) {
        int newCapacity    = STACK->capacity * 2;
        
        STACK->memoryChunk = (stack_t *)realloc(STACK->memoryChunk,
                                                sizeof(stack_t) * ((size_t)newCapacity + COUNT_OF_CANARIES));

        customAssert(STACK->memoryChunk != NULL, STACK_DATA_NULL_POINTER);

        DATA_BEGIN_CANARY_INITIALIZE(STACK->memoryChunk);
        DATA_END_CANARY_INITIALIZE  (STACK->memoryChunk, newCapacity + CANARY_SHIFT);

        STACK->capacity    *= 2;
        STACK->data         = STACK->memoryChunk + CANARY_SHIFT;

        errorCode = stackCheck(STACK);
        customWarning(errorCode == STACK_NO_ERROR, errorCode);

        stackFillPoison(STACK); // TODO customRealloc (realloc + fill)
    }

    else if (changeMemoryMode == DUMP_MEMORY) {
        int newCapacity    = STACK->capacity / 2;

        STACK->memoryChunk = (stack_t *)realloc(STACK->memoryChunk,
                                         sizeof(stack_t) * ((size_t)newCapacity + COUNT_OF_CANARIES));

        customAssert(STACK->memoryChunk != NULL, STACK_DATA_NULL_POINTER);

        DATA_BEGIN_CANARY_INITIALIZE(STACK->memoryChunk);
        DATA_END_CANARY_INITIALIZE  (STACK->memoryChunk, newCapacity + CANARY_SHIFT);

        STACK->capacity    /= 2;
        STACK->data         = STACK->memoryChunk + CANARY_SHIFT;
        
        errorCode = stackCheck(STACK);
        customWarning(errorCode == STACK_NO_ERROR, errorCode);

        stackFillPoison(STACK); // TODO customRealloc (realloc + fill)
    }

    DUMP_(STACK);

    errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
}

stackError stackCheck(stack *STACK) {
    if (STACK == NULL) {
        STACK->errorStatus |= 1 << STACK_NULL_POINTER;
        return STACK_NULL_POINTER;
    }

    if (STACK->memoryChunk == NULL) {
        STACK->errorStatus |= 1 << STACK_DATA_NULL_POINTER;
        return STACK_DATA_NULL_POINTER;
    }

    if (STACK->size > STACK->capacity) {
        STACK->errorStatus |= 1 << STACK_OVERFLOW;
        return STACK_OVERFLOW;
    }

    if (STACK->size < 0) {
        STACK->errorStatus |= 1 << STACK_ANTI_OVERFLOW;
        return STACK_ANTI_OVERFLOW;
    }

    if (STACK->capacity < 0) {
        STACK->errorStatus |= 1 << STACK_BAD_CAPACITY;
        return STACK_BAD_CAPACITY;
    }

    #ifndef _NDEBUG

    if (STACK->leftCanary != CANARY || STACK->rightCanary != CANARY) {
        STACK->errorStatus |= 1 << STACK_STRUCT_BAD_CANARY;
        return STACK_STRUCT_BAD_CANARY;
    }

    #endif

    if (STACK->memoryChunk != NULL) {
        int canaryStatus = 0;
        CHECK_CANARY();
        if (canaryStatus != 0) {
            STACK->errorStatus |= 1 << STACK_DATA_BAD_CANARY;
            return STACK_DATA_BAD_CANARY;
        }
    }

    return STACK_NO_ERROR;
}

stackError printStack(stack *STACK) {
    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    for (int index = 0; index < STACK->capacity; index++) {
        printf("[%d, %p] = %" SPECIFICATOR_TYPE "\n", index, STACK->data + index, STACK->data[index]);
    }

    return STACK_NO_ERROR;
}

void printBinaryErrorStatus(int errorBinaryCode) {
    if (errorBinaryCode > 1) {
            printBinaryErrorStatus(errorBinaryCode >> 1);
        }

    printf("%d", errorBinaryCode % 2);
}