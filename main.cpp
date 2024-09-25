#include <cstdio>

#include "stack.h"

#define INIT(st) st.bornFileName = __FILE__, st.bornLine = __LINE__, st.funcPrototype = __PRETTY_FUNCTION__, st.dumpFile = "stackDump.txt"

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

    return 0;
}