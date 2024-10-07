#include <cstdint>
#include <cstdlib>

#include "stackHash.h"
#include "stack.h"
#include "customWarning/customWarning.h"

uint64_t djb2Hash(stack *stack) {
    customWarning(stack != NULL, 1); // TODO check

    uint64_t hash = hashValue;
    uint64_t code = 0;
    for (size_t index = 0; index < stack->size; index++) {
        if (index == 0) {
            code = stack->data[index];
        }
        
        else {
            code ^= stack->data[index];
        }
        
        hash = ((hash << 5) + hash) + code;
    }

    return hash;
}

int djb2HashCheck(stack *stack) {
    customWarning(stack != NULL, 1); // TODO check
    
    uint64_t newHash = djb2Hash(stack);

    if (newHash != stack->hash) {
        return 1; // TODO return value
    }
    
    stack->hash = newHash;
    return 0;
}
