#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "privateStack.h"
#include "customExits.h"
#include "stack.h"

extern stack *STACK_PTR;

stack *createPrivateStack() {
    stack *STACK = (stack *)calloc(1, sizeof(stack));

    STACK_PTR    = STACK;

    if (STACK == NULL) {
        assert("STACK POINTER IS NULL" && 0);
    }

    *STACK = {};
    
    return STACK;
}

stackError initializePrivateStack(stack *STACK, const char *fileName, int line, const char *function) {
    customAssert(STACK == STACK_PTR, STACK_NULL_POINTER);

    #ifndef _NDEBUG
    STACK->leftCanary  = CANARY;
    STACK->rightCanary = CANARY;
    #endif

    STACK->bornFileName      = fileName;
    STACK->bornLine          = line;
    STACK->bornFuncPrototype = function;

    STACK->errorStatus       = 0;

    setDumpFileName(STACK);
    stackInitialize(STACK, START_STACK_SIZE);

    customAssert(STACK->memoryChunk != NULL, STACK_DATA_NULL_POINTER);

    DATA_BEGIN_CANARY_INITIALIZE(STACK->memoryChunk);
    DATA_END_CANARY_INITIALIZE  (STACK->memoryChunk, STACK->capacity + 1);

    stackError errorCode = stackCheck(STACK);
    customWarning(errorCode == STACK_NO_ERROR, errorCode);

    return STACK_NO_ERROR;
}

stackError destructPrivateStack(stack *STACK) {
    customWarning(STACK == STACK_PTR, STACK_NULL_POINTER);

    stackDestruct(STACK);

    free(STACK->dumpFile);
    free(STACK->memoryChunk);

    printf("[ERROR BINARY CODE]: ");
    printBinaryErrorStatus(STACK->errorStatus);
    printf("\n");
    printErrorDescription(STACK->errorStatus);

    STACK->errorStatus = 0;

    free(STACK);

    return STACK_NO_ERROR;
}

stackError setDumpFileName(stack *STACK) {
    customWarning(STACK == STACK_PTR, STACK_NULL_POINTER);

    char *buffer           = (char *)calloc(NAME_BUFFER_SIZE, sizeof(char));
    customWarning(buffer != NULL, DUMP_FILE_NAME_NULL_POINTER);
    
    const time_t timer     = time(NULL);
    tm *now                = localtime(&timer);
    const char *timeChar   = asctime(now);  
    size_t timeCharLength  = strlen(timeChar) - 1;
    const char *folderName = "dumps/";
    size_t systemCmdLength = strlen("mkdir ") + strlen(folderName) + 1;
    char *systemCmdBuffer  = (char *)calloc(systemCmdLength, sizeof(char));
    customWarning(systemCmdBuffer != NULL, CMD_BUFFER_NULL_POINTER);
    strcpy(systemCmdBuffer, "mkdir ");
    const char *systemCmd  = strcat(systemCmdBuffer, folderName);

    system(systemCmd);

    strcpy(buffer, folderName);
    strncpy(buffer + strlen(folderName), timeChar, timeCharLength);
    strcat(buffer, ".html");

    STACK->dumpFile        = buffer;

    free(systemCmdBuffer);
    systemCmdBuffer = NULL;

    return STACK_NO_ERROR;
}