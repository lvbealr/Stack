#ifndef STACK_H_
#define STACK_H_

typedef int stack_t;

#define GET_OBJECT_LAST_INFO(st) st->lastUseFileName = __FILE__; st->lastUseLine = __LINE__

#define DUMP_(st) {GET_OBJECT_LAST_INFO(st); \
                   stackDumpHtml(st);            \
                  }

#define ON_DEBUG(...) __VA_ARGS__

struct stack {
    ON_DEBUG(const char *bornFileName);
    ON_DEBUG(int         bornLine);
    ON_DEBUG(const char *funcPrototype);
    ON_DEBUG(const char *lastUseFileName);
    ON_DEBUG(int         lastUseLine);
    const char *dumpFile;
    size_t      size;
    size_t      capacity;
    stack_t    *data;
};

#undef ON_DEBUG // ?????????????????????

const int DUMP_MEMORY = 0;
const int ADD_MEMORY  = 1;

const size_t START_STACK_SIZE = 100;

const stack_t BAD_VALUE = -666; // CHANGE IN DEPENDENCE OF STACK_T TYPE

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
int stackInitialize(stack *stack, int size);
int stackDestruct(stack *stack);
int stackPush(stack *stack, stack_t value);
int stackPop(stack *stack, stack_t *variable);
static inline int stackResize(stack *stack, const int mode);
int stackDump(stack *stack);
int stackCheck(stack *stack);
// FUNCTION PROTOTYPES //

#endif // STACK_H_

