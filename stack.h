#ifndef STACK_H_
#define STACK_H_

#include <cstdint>
#include <cassert>

// TODO DATA_CANARY UINT64_T BUT MEMORY CELL FOR STACK_T = INT

typedef int stack_t;
const uint64_t CANARY = 5051;

#ifndef _NDEBUG
    #define GET_OBJECT_LAST_INFO(st) st->lastUseFileName       = __FILE__;     \
                                     st->lastUseLine           = __LINE__;     \
                                     st->lastUseFuncPrototype  = __FUNCTION__; 

    #define DUMP_(st) {                                                        \
        GET_OBJECT_LAST_INFO(st);                                              \
        stackDumpHtml(st);                                                     \
    }

    // #define ON_DEBUG(...) __VA_ARGS__

    #define CONST_CANARY_INITIALIZE(constCanary)                               \
        const uint64_t constCanary = CANARY;                                   \
                
    #define DATA_BEGIN_CANARY_INITIALIZE(stack)                                \
        *(stack)                   = CANARY;                                   \
    
    #define DATA_END_CANARY_INITIALIZE(stack, canaryPosition)                  \
        *(stack + canaryPosition)  = CANARY;                                   \
                
    #define CANARY_SIZE(canary)                                                \
        sizeof(canary) / sizeof(stack_t)                                       \
                
    #define INFO_CANARY_CHECK(infoCanary)                                      \
        infoCanary != CANARY                                                   \
                
    #define DATA_CANARY_CHECK(dataCanary, dataCanaryPosition)                  \
        *(dataCanary + dataCanaryPosition) != CANARY                           \
                
    #define CHECK_CANARY()                                                     \
        if (INFO_CANARY_CHECK(stack->leftCanary)            ||                 \
            INFO_CANARY_CHECK(stack->rightCanary)           ||                 \
            DATA_CANARY_CHECK(stack->memoryChunk, 0)        ||                 \
            DATA_CANARY_CHECK(stack->memoryChunk,                              \
                              stack->capacity + CANARY_SIZE(CANARY)))          \
            {                                                                  \
                printf("FUCK CANARY");                                         \
                assert(0);                                                     \
            }                                                                  \

#else
    #define GET_OBJECT_LAST_INFO(st)
    #define DUMP_(st)
    // #define ON_DEBUG(...)
    #define CONST_CANARY_INITIALIZE(constCanary)
    #define DATA_BEGIN_CANARY_INITIALIZE(stack)
    #define DATA_END_CANARY_INITIALIZE(stack, canaryPosition)
    #define CANARY_SIZE(canary)
    #define INFO_CANARY_CHECK(infoCanary)
    #define DATA_CANARY_CHECK(dataCanary, dataCanaryPosition)

#endif // NDEBUG

struct stack {
    const char *bornFileName;
    int         bornLine;
    const char *bornFuncPrototype;
    const char *lastUseFileName;
    int         lastUseLine;
    const char *lastUseFuncPrototype;
    char       *dumpFile;
    CONST_CANARY_INITIALIZE(leftCanary);
    size_t      size;
    size_t      capacity;
    CONST_CANARY_INITIALIZE(rightCanary);
    stack_t    *memoryChunk;
    stack_t    *data;
};

enum changeMemory {
    DUMP_MEMORY = 0,
    ADD_MEMORY  = 1
};

const size_t START_STACK_SIZE = 20;
const size_t NAME_BUFFER_SIZE = 40;

const stack_t POISON_VALUE = -666; // CHANGE IN DEPENDENCE OF STACK_T TYPE

// TODO stackError functions
// TODO check stackErrors
enum stackError {
    STACK_OVERFLOW     = 0,
    STACK_UNDERFLOW    = 1,
    STACK_BAD_POINTER  = 2,
    STACK_BAD_CAPACITY = 3,
    STACK_BAD_SIZE     = 4,
    STACK_BAD_VALUE    = 5
};

// FUNCTION PROTOTYPES //
int stackInitialize (stack *stack, size_t size);
int stackFillPoison (stack *stack, size_t fillSize);
int stackDestruct   (stack *stack);
int stackPush       (stack *stack, stack_t value);
int stackPop        (stack *stack, stack_t *variable);
int stackResize     (stack *stack, const changeMemory changeMemoryMode);
int stackCheck      (stack *stack);
// FUNCTION PROTOTYPES //

#endif // STACK_H_

