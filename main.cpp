#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "stack.h"

// #define SHOW_HTML() system("xdg-open ~/Stack/stackDump.html"); system("rm -r ~/Stack/stackDump.html")

#ifndef _NDEBUG
    // TODO what does init mean?
    #define INIT_STACK(st) st.bornFileName = __FILE__, st.bornLine = __LINE__, st.funcPrototype = __PRETTY_FUNCTION__

#else
    #define INIT(st)
    #define SHOW_HTML()
    
#endif

static int setDumpFileName(stack *stack) {
    customWarning(stack != NULL, 1);

    char buffer[32]       = {};
    const time_t timer    = time(NULL);
    tm *now               = localtime(&timer);
    const char *timeChar  = asctime(now);  
    size_t timeCharLength = strlen(timeChar) - 1;

    strncpy(buffer, timeChar, timeCharLength);
    strcat(buffer, ".html");

    for (size_t index = 0; index < sizeof(buffer); index++) {
        if (buffer[index] == ' ') {
            buffer[index] = '-';
        }
    }

    stack->dumpFile = buffer;

    printf("ВНУТРИ setDumpFileName: %s\n", stack->dumpFile);

    return 0;
}

int main(int argc, char *argv[]) {

    stack Stack = {INIT_STACK(Stack)};
    setDumpFileName(&Stack);

    printf("ДО stackInitialize: %s\n", Stack.dumpFile);
    stackInitialize(&Stack, 20);
    printf("ПОСЛЕ stackInitialize: %s\n", Stack.dumpFile);
    // TODO sprintf to name time

    ////////////////////////
    for (int testIndex = 0; testIndex < 25; testIndex++) {
        stackPush(&Stack, testIndex);
    }
    
    stack_t x = 0;

    stackPop(&Stack, &x);
    stackPop(&Stack, &x);

    stackPush(&Stack, 999);
    ////////////////////////

    stackDestruct(&Stack);

    // SHOW_HTML();

    return 0;
}