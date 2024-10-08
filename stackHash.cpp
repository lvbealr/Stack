#include <cstdint>
#include <cstdlib>

#include "stackHash.h"
#include "stack.h"
#include "customExits.h"

#ifndef _NDEBUG

uint64_t djb2Hash(stack *STACK) {
    if (STACK == NULL) {
        exit(0);
    }

    uint64_t hash = hashValue;
    uint64_t code = 0;

    for (int index = 0; index < STACK->size; index++) {
        if (index == 0) {
            code = (uint64_t)STACK->data[index];
        }
        
        else {
            code ^= (uint64_t)STACK->data[index];
        }
        
        hash = ((hash << 5) + hash) + code;
    }

    return hash;
}

stackError djb2HashCheck(stack *STACK) {
    customAssert(STACK != NULL, STACK_NULL_POINTER);
    
    uint64_t newHash = djb2Hash(STACK);

    if (newHash != STACK->hash) {
        return STACK_NO_ERROR;
    }

    STACK->hash = newHash;

    return STACK_NO_ERROR;
}

#endif