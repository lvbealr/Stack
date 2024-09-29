#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "stack.h"
#include "customWarning/customWarning.h"

#ifndef _NDEBUG
    #define INIT_STACK(st) st.bornFileName      = __FILE__,                \
                           st.bornLine          = __LINE__,                \
                           st.bornFuncPrototype = __PRETTY_FUNCTION__

#else
    #define INIT_STACK(st)
    
#endif

static int setDumpFileName(stack *stack) {
    customWarning(stack != NULL, 1);

    char *buffer           = (char *)calloc(NAME_BUFFER_SIZE, sizeof(char));
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

int main() {

    stack Stack = {INIT_STACK(Stack)};
    setDumpFileName(&Stack);

    stackInitialize(&Stack, START_STACK_SIZE);

    //////////////////////////////////////////////////////////
    for (int testIndex = 0; testIndex < 25; testIndex++) {  //
        stackPush(&Stack, testIndex);                       //
    }                                                       //
                                                            //
    stack_t x = 0;                                          // 
                                                            //
    stackPop(&Stack, &x);                                   //
    stackPop(&Stack, &x);                                   //
                                                            //
    stackPush(&Stack, 999);                                 //
    //////////////////////////////////////////////////////////

    stackDestruct(&Stack);

    return 0;
}