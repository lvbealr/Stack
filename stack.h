#ifndef STACK_H_
#define STACK_H_

typedef int stack_t;

#ifndef _NDEBUG
    #define GET_OBJECT_LAST_INFO(st) st->lastUseFileName       = __FILE__;     \
                                     st->lastUseLine           = __LINE__;     \
                                     st->lastUseFuncPrototype  = __FUNCTION__; 

    #define DUMP_(st) {           \
        GET_OBJECT_LAST_INFO(st); \
        stackDumpHtml(st);        \
    }

    #define ON_DEBUG(...) __VA_ARGS__

#else
    #define GET_OBJECT_LAST_INFO(st)
    #define DUMP_(st)
    #define ON_DEBUG(...)

#endif // NDEBUG

struct stack {
    const char *bornFileName;
    int         bornLine;
    const char *bornFuncPrototype;
    const char *lastUseFileName;
    int         lastUseLine;
    const char *lastUseFuncPrototype;
    char       *dumpFile;
    size_t      size;
    size_t      capacity;
    stack_t    *data;
};

enum changeMemory {
    DUMP_MEMORY = 0,
    ADD_MEMORY  = 1
};

const size_t START_STACK_SIZE = 100;
const size_t NAME_BUFFER_SIZE = 40;

const stack_t POISON_VALUE = -666; // CHANGE IN DEPENDENCE OF STACK_T TYPE

// DO stackError functions !!!
enum stackError {
    STACK_OVERFLOW     = 0,
    STACK_UNDERFLOW    = 1,
    STACK_BAD_POINTER  = 2,
    STACK_BAD_CAPACITY = 3,
    STACK_BAD_SIZE     = 4,
    STACK_BAD_VALUE    = 5
};

// FUNCTION PROTOTYPES //
int stackInitialize(stack *stack, size_t size);
int stackFillPoison(stack *stack, size_t fillSize);
int stackDestruct(stack *stack);
int stackPush(stack *stack, stack_t value);
int stackPop(stack *stack, stack_t *variable);
int stackResize(stack *stack, const changeMemory changeMemoryMode);
int stackCheck(stack *stack);
// FUNCTION PROTOTYPES //

#endif // STACK_H_

