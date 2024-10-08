#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "customExits.h"
#include "privateStack.h"

int main() {
    stack *stack = createPrivateStack();
    initializePrivateStack(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    //////////////////////////////////////////////////////////
    for (int testIndex = 0; testIndex < 25; testIndex++) {  //
        stackPush(stack, testIndex);                        //
    }                                                       //
                                                            //
                                                            //
    stack_t x = 0;                                          //
                                                            //
    for (size_t index = 0; index < 30; index++) {           //
        stackPop(stack, &x);                                //
    }                                                       //
                                                            //
    stackPush(stack, 999);                                  //
    //////////////////////////////////////////////////////////

    destructPrivateStack(stack);

    return 0;
}