#include <ctime>
#include <cstdlib>
#include <cstring>

#include "privateStack.h"
#include "customWarning/customWarning.h"
#include "stack.h"

stack *initializePrivateStack(const char *fileName, int line, const char *function) {
    stack *Stack = (stack *)calloc(1, sizeof(stack));
    assert(Stack != NULL); // TODO USE MY ASSERT

    Stack->leftCanary  = CANARY;
    Stack->rightCanary = CANARY;

    Stack->bornFileName      = fileName;
    Stack->bornLine          = line;
    Stack->bornFuncPrototype = function;

    setDumpFileName(Stack);
    stackInitialize(Stack, START_STACK_SIZE);

    DATA_BEGIN_CANARY_INITIALIZE(Stack->memoryChunk);
    DATA_END_CANARY_INITIALIZE  (Stack->memoryChunk, Stack->capacity + 1);

    return Stack;
}

int destructPrivateStack(stack *stack) {
    customWarning(stack != NULL, 1);

    free(stack->dumpFile);
    free(stack->memoryChunk);
    free(stack);

    return 0;
}

int setDumpFileName(stack *stack) {
    customWarning(stack != NULL, 1);

    char *buffer           = (char *)calloc(NAME_BUFFER_SIZE, sizeof(char));
    customWarning(buffer != NULL, 1);
    
    const time_t timer     = time(NULL);
    tm *now                = localtime(&timer);
    const char *timeChar   = asctime(now);  
    size_t timeCharLength  = strlen(timeChar) - 1;
    const char *folderName = "dumps/";

    strcpy(buffer, folderName);
    strncpy(buffer + strlen(folderName), timeChar, timeCharLength);
    strcat(buffer, ".html");

    stack->dumpFile        = buffer;

    return 0;
}