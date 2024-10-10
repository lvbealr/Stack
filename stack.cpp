#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "stackDump.h"
#include "privateStack.h"
#include "stack.h"
#include "customExits.h"
#include "stackHash.h"
#include "colorPrint.h"

extern stack *STACK_PTR;

#define SPECIFICATOR_TYPE "d"

static stackError stackResize(stack *stack, const changeMemory changeMemoryMode);

///////////////////////////////////////////////////////////
// stackError errorCode = stackCheck(stack);             //
// customWarning(errorCode == STACK_NO_ERROR, errorCode);//
///////////////////////////////////////////////////////////

stackError stackInitialize(stack *STACK, int capacity) {
    customAssert(STACK    != NULL, STACK_NULL_POINTER);
    customAssert(capacity >= 0,    STACK_BAD_CAPACITY);

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
    customAssert(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
}

stackError stackFillPoison(stack *STACK) {
    stackError errorCode = stackCheck(STACK);
    customAssert(errorCode == STACK_NO_ERROR, errorCode);

    for (int index = STACK->size; index < STACK->capacity; index++) {
        STACK->data[index] = POISON_VALUE;
    }

    DUMP_(STACK);

    errorCode = stackCheck(STACK);
    customAssert(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
}

stackError stackDestruct(stack *STACK) {
    stackError errorCode = stackCheck(STACK);
    customAssert(errorCode == STACK_NO_ERROR, errorCode);

    STACK->size     = 0;
    STACK->capacity = 0;
    #ifndef _NDEBUG
    STACK->hash     = 0;
    #endif
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
    customAssert(errorCode == STACK_NO_ERROR, errorCode);
    customAssert(value != POISON_VALUE, INVALID_INPUT_VALUE);

    #ifndef _NDEBUG
    customAssert(!djb2HashCheck(STACK), STACK_BAD_HASH);
    #endif

    if (STACK->size >= STACK->capacity) {
        stackResize(STACK, ADD_MEMORY);
    }

    STACK->data[STACK->size++] = value;

    #ifndef _NDEBUG
    STACK->hash = djb2Hash(STACK);
    customAssert(!djb2HashCheck(STACK), STACK_BAD_HASH);
    #endif

    DUMP_(STACK);

    #ifndef _NDEBUG
    errorCode = stackCheck(STACK);
    customAssert(errorCode == STACK_NO_ERROR, errorCode);
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
    customAssert(errorCode == STACK_NO_ERROR, errorCode);

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
        customAssert(errorCode == STACK_NO_ERROR, errorCode);

        stackFillPoison(STACK);
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
        customAssert(errorCode == STACK_NO_ERROR, errorCode);

        stackFillPoison(STACK);
    }

    DUMP_(STACK);

    errorCode = stackCheck(STACK);
    customAssert(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
}

stackError stackCheck(stack *STACK) {
    if (STACK == NULL) {
        STACK->errorStatus |= STACK_NULL_POINTER;
        return STACK_NULL_POINTER;
    }

    if (STACK != STACK_PTR) {
        STACK_PTR->errorStatus |= STACK_INVALID_POINTER;
        return STACK_INVALID_POINTER;
    }

    if (STACK_PTR->memoryChunk == NULL || STACK_PTR->data == NULL) {
        STACK_PTR->errorStatus |= STACK_DATA_NULL_POINTER;
        return STACK_DATA_NULL_POINTER;
    }

    if (STACK_PTR->size > STACK_PTR->capacity) {
        STACK_PTR->errorStatus |= STACK_OVERFLOW;
        return STACK_OVERFLOW;
    }

    if (STACK_PTR->size < 0) {
        STACK_PTR->errorStatus |= STACK_ANTI_OVERFLOW;
        return STACK_ANTI_OVERFLOW;
    }

    if (STACK_PTR->capacity < 0) {
        STACK_PTR->errorStatus |= STACK_BAD_CAPACITY;
        return STACK_BAD_CAPACITY;
    }

    #ifndef _NDEBUG

    if (STACK_PTR->leftCanary != CANARY || STACK_PTR->rightCanary != CANARY) {
        STACK_PTR->errorStatus |= STACK_STRUCT_BAD_CANARY;
        return STACK_STRUCT_BAD_CANARY;
    }

    int canaryStatus = 0;
    CHECK_CANARY();
    if (canaryStatus != 0) {
        STACK_PTR->errorStatus |= STACK_DATA_BAD_CANARY;
        return STACK_DATA_BAD_CANARY;
    }

    #endif

    return STACK_NO_ERROR;
}

stackError printStack(stack *STACK) {
    stackError errorCode = stackCheck(STACK);
    customAssert(errorCode == STACK_NO_ERROR, errorCode);

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

#define DESCR_(err) {                                                                   \
    case err:                                                                           \
        customPrint(red, bold, bgDefault, "[FAIL: %d] " #err, errorBinaryCode & shift); \
        shift <<= 1;                                                                    \
        break;                                                                          \
}

void printErrorDescription(int errorBinaryCode) {
    int shift = 1;

    while (errorBinaryCode >= shift) {
        switch (errorBinaryCode & shift) {
            DESCR_(STACK_NULL_POINTER);
            DESCR_(STACK_INVALID_POINTER);
            DESCR_(STACK_DATA_NULL_POINTER);
            DESCR_(STACK_OVERFLOW);
            DESCR_(STACK_ANTI_OVERFLOW);
            DESCR_(STACK_BAD_CAPACITY);
            DESCR_(STACK_STRUCT_BAD_CANARY);
            DESCR_(STACK_DATA_BAD_CANARY);
            DESCR_(STACK_BAD_HASH);
            DESCR_(INVALID_INPUT_VALUE);
            DESCR_(STACK_BAD_SIZE);
            DESCR_(POP_VARIABLE_NULL_POINTER);
            DESCR_(DUMP_FILE_NAME_NULL_POINTER);
            DESCR_(CMD_BUFFER_NULL_POINTER);
            
            case STACK_NO_ERROR:
            default:
                shift <<= 1;
                break;
        }
    }
}

#undef DESCR_