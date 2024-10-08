#ifndef STACK_H_
#define STACK_H_

#include <cstdint>

#include "privateStack.h"

typedef int stack_t;
const stack_t CANARY = 5051;

#ifndef _NDEBUG
    #define GET_OBJECT_LAST_INFO(st) st->lastUseFileName       = __FILE__;     \
                                     st->lastUseLine           = __LINE__;     \
                                     st->lastUseFuncPrototype  = __FUNCTION__;

    #define DUMP_(st) {                                                        \
        GET_OBJECT_LAST_INFO(st);                                              \
        stackDumpHtml(st);                                                     \
    }

    #define CANARY_INITIALIZE(canary)                                          \
        stack_t canary = CANARY;                                               \

    #define DATA_BEGIN_CANARY_INITIALIZE(memoryCell)                           \
        stack_t *beginCanaryCell = (stack_t *)memoryCell;                      \
        *beginCanaryCell          = CANARY;                                    \
    
    #define DATA_END_CANARY_INITIALIZE(memoryCell, canaryPosition)             \
        stack_t *endCanaryCell    = memoryCell + canaryPosition;               \
        *endCanaryCell            = CANARY;                                    \

    #define CANARY_SIZE(canary)                                                \
        sizeof(canary) / sizeof(stack_t)                                       \
                
    #define INFO_CANARY_CHECK(infoCanary)                                      \
        infoCanary != CANARY                                                   \
                
    #define DATA_CANARY_CHECK(dataCanary, dataCanaryPosition)                  \
        *(dataCanary + dataCanaryPosition) != CANARY                           \
                
    #define CHECK_CANARY()                                                     \
        if (INFO_CANARY_CHECK(STACK->leftCanary)            ||                 \
            INFO_CANARY_CHECK(STACK->rightCanary)           ||                 \
            DATA_CANARY_CHECK(STACK->memoryChunk, 0)        ||                 \
            DATA_CANARY_CHECK(STACK->memoryChunk,                              \
                              STACK->capacity + 1))                            \
            {                                                                  \
                canaryStatus = 1;                                              \
            }                                                                  \

    #define CANARY_DESTRUCT() {                                                \
        STACK->leftCanary  = 0;                                                \
        STACK->rightCanary = 0;                                                \
    }                                                                          \

#else
    #define GET_OBJECT_LAST_INFO(st)
    #define DUMP_(st)
    #define CANARY_INITIALIZE(canary)
    #define DATA_BEGIN_CANARY_INITIALIZE(stack)
    #define DATA_END_CANARY_INITIALIZE(stack, canaryPosition)
    #define CANARY_SIZE(canary) 0
    #define INFO_CANARY_CHECK(infoCanary)
    #define DATA_CANARY_CHECK(dataCanary, dataCanaryPosition)
    #define CHECK_CANARY()
    #define CANARY_DESTRUCT()

#endif // NDEBUG

// TODO INFO STACK STRUCT

struct stack {
    CANARY_INITIALIZE(leftCanary);
    const char *bornFileName;
    int         bornLine;
    const char *bornFuncPrototype;
    const char *lastUseFileName;
    int         lastUseLine;
    const char *lastUseFuncPrototype;
    char       *dumpFile;
    int         errorStatus;
    int         size;
    int         capacity;
    stack_t    *memoryChunk;
    stack_t    *data;
    #ifndef _NDEBUG
    uint64_t    hash;
    #endif
    CANARY_INITIALIZE(rightCanary);
};

enum changeMemory { 
    DUMP_MEMORY = 0,
    ADD_MEMORY  = 1
};

const int     NAME_BUFFER_SIZE  = 40;
const stack_t POISON_VALUE      = -666;
const size_t  COUNT_OF_CANARIES = 2;
const size_t  CANARY_SHIFT      = 1;
// TODO stackError functions
// TODO check stackErrors

// FUNCTION PROTOTYPES //
stackError stackFillPoison        (stack *STACK);
stackError stackCheck             (stack *STACK);
stackError printStack             (stack *STACK);
void       printBinaryErrorStatus (int errorBinaryCode);
// FUNCTION PROTOTYPES //

#endif // STACK_H_

