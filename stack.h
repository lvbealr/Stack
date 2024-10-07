#ifndef STACK_H_
#define STACK_H_

#include <cstdint>
#include <cassert> // TODO USE MY ASSERT INSTEAD OF ASSERT

// TODO HASH IN DEPENDENCE OF TIME

typedef int stack_t;
const stack_t CANARY = 5051; // TODO ALIGN MEMORY!!!

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
        stack_t *endCanaryCell   = (stack_t *)(memoryCell + canaryPosition);   \
        *endCanaryCell            = CANARY;                                    \

    #define CANARY_SIZE(canary)                                                \
        sizeof(canary) / sizeof(stack_t)                                       \
                
    #define INFO_CANARY_CHECK(infoCanary)                                      \
        infoCanary != CANARY                                                   \
                
    #define DATA_CANARY_CHECK(dataCanary, dataCanaryPosition)                  \
        *(stack_t *)(dataCanary + dataCanaryPosition) != CANARY                \
                
    #define CHECK_CANARY()                                                     \
        if (INFO_CANARY_CHECK(stack->leftCanary)            ||                 \
            INFO_CANARY_CHECK(stack->rightCanary)           ||                 \
            DATA_CANARY_CHECK(stack->memoryChunk, 0)        ||                 \
            DATA_CANARY_CHECK(stack->memoryChunk,                              \
                              stack->capacity + 1))                            \
            {                                                                  \
                printf("FUCK CANARY!\n");                                      \
                fflush(stdout);                                                \
                assert(0);                                                     \
            }                                                                  \

    #define CANARY_DESTRUCT() {                                                \
        stack->leftCanary  = 0;                                                \
        stack->rightCanary = 0;                                                \
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
    uint32_t    stackErrorStatus;
    size_t      size;
    size_t      capacity;
    stack_t    *memoryChunk;
    stack_t    *data;
    uint64_t    hash;
    CANARY_INITIALIZE(rightCanary);
};

enum changeMemory {
    DUMP_MEMORY = 0,
    ADD_MEMORY  = 1
};

const size_t  NAME_BUFFER_SIZE  = 40;
const stack_t POISON_VALUE      = -666;
const size_t  COUNT_OF_CANARIES = 2;
const size_t  CANARY_SHIFT      = 1;
// TODO stackError functions
// TODO check stackErrors

enum stackError {
    STACK_NO_ERROR                = 0,
    STACK_INITIALIZED             = 1 <<  0,
    STACK_NOT_INITIALIZED         = 1 <<  1,
    STACK_BAD_POINTER             = 1 <<  2,
    STACK_OVERFLOW                = 1 <<  3,
    STACK_UNDERFLOW               = 1 <<  4,
    STACK_BAD_CAPACITY            = 1 <<  5,
    STACK_BAD_SIZE                = 1 <<  6,
    STACK_STRUCT_BAD_LEFT_CANARY  = 1 <<  7,
    STACK_STRUCT_BAD_RIGHT_CANARY = 1 <<  8,
    STACK_BAD_LEFT_CANARY         = 1 <<  9,
    STACK_BAD_RIGHT_CANARY        = 1 << 10,
    STACK_BAD_HASH                = 1 << 11
};

// FUNCTION PROTOTYPES //
int        stackFillPoison (stack *stack);
static int stackResize     (stack *stack, const changeMemory changeMemoryMode);
int        stackCheck      (stack *stack);
int        printStack      (stack *stack);
// FUNCTION PROTOTYPES //

#endif // STACK_H_

