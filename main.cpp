#include <cstdio>
#include <cstdlib>

#include "stack.h"

#ifndef _NDEBUG
    #define INIT(st) st.bornFileName = __FILE__, st.bornLine = __LINE__, st.funcPrototype = __PRETTY_FUNCTION__, st.dumpFile = "stackDump.html"
    #define SHOW_HTML() system("xdg-open ~/Stack/stackDump.html"); system("rm -r ~/Stack/stackDump.html")

#else
    #define INIT(st)
    #define SHOW_HTML()
    
#endif

int main(int argc, char *argv[]) {
    stack Stack = {INIT(Stack)};

    stackInitialize(&Stack, 20);

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

    SHOW_HTML();

    return 0;
}