#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "stack.h"
#include "privateStack.h"

int main() {
    stack *STACK = createPrivateStack();
    initializePrivateStack(STACK, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    //////////////////////////////////////////////////////////
    for (int testIndex = 0; testIndex < 25; testIndex++) {  //
        stackPush(STACK, testIndex);                        //
    }                                                       //
                                                            //
                                                            //
    stack_t x = 0;                                          //
                                                            //
    for (size_t index = 0; index < 30; index++) {           //
        stackPop(STACK, &x);                                //
    }                                                       //
                                                            //
    stackPush(STACK, 999);                                  //
    //////////////////////////////////////////////////////////

    destructPrivateStack(STACK);

    return 0;
}