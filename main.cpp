#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "customWarning/customWarning.h"
#include "privateStack.h"

int main() {

    stack *Stack = initializePrivateStack(__FILE__, __LINE__, __PRETTY_FUNCTION__);

    //////////////////////////////////////////////////////////
    for (int testIndex = 0; testIndex < 25; testIndex++) {  //
        stackPush(Stack, testIndex);                        //
    }                                                       //
                                                            //
                                                            //
    stack_t x = 0;                                          //
                                                            //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
    stackPop(Stack, &x);                                    //
                                                            //
    stackPush(Stack, 999);                                  //
    //////////////////////////////////////////////////////////

    destructPrivateStack(Stack);

    return 0;
}