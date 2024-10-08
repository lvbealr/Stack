#ifndef STACK_HASH_H
#define STACK_HASH_H

#include "stack.h"

const uint64_t hashValue = 5381;

// FUNCTION PROTOTYPES
uint64_t   djb2Hash      (stack *stack);
stackError djb2HashCheck (stack *stack);
// FUNCTION PROTOTYPES 

#endif // STACK_HASH_H