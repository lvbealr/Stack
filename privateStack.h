#ifndef PRIVATE_STACK_H_
#define PRIVATE_STACK_H_

#include <cstdlib>

typedef int stack_t;

struct stack;

const int START_STACK_SIZE = 20;

enum stackError {
    STACK_NO_ERROR                =  0,
    STACK_NULL_POINTER            =  1,
    STACK_DATA_NULL_POINTER       =  2,
    STACK_OVERFLOW                =  3,
    STACK_ANTI_OVERFLOW           =  4,
    STACK_BAD_CAPACITY            =  5,
    STACK_STRUCT_BAD_CANARY       =  6,
    STACK_DATA_BAD_CANARY         =  7,
    STACK_BAD_HASH                =  8,
    INVALID_INPUT_VALUE           =  9,
    STACK_BAD_SIZE                = 10,
    POP_VARIABLE_NULL_POINTER     = 11,
    DUMP_FILE_NAME_NULL_POINTER   = 12,
    CMD_BUFFER_NULL_POINTER       = 13
};

// FUNCTION PROTOTYPES //
stackError stackInitialize         (stack *stack, int      capacity);
stackError stackDestruct           (stack *stack);
stackError stackPush               (stack *stack, stack_t  value);
stackError stackPop                (stack *stack, stack_t *variable);
stackError destructPrivateStack    (stack *stack);
stackError setDumpFileName         (stack *stack);
stack     *createPrivateStack      ();
stackError initializePrivateStack  (stack *stack, const char *fileName, const int line, const char *function);
// FUNCTION PROTOTYPES //

#endif