#include <cstdio>

// #include "time.h" // ADD TIME

#include "stackDump.h"
#include "customWarning/customWarning.h"

const char *startHtml = R"(<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>Document</title></head><body>)";
const char *endHtml   = R"(</body></html>)";

#define SPECIFICATOR_TYPE "d"

int stackDump(stack *stack) {
    FILE *outputFile = fopen(stack->dumpFile, "a");
    customWarning(outputFile != NULL, 1);

    fprintf(outputFile, "stack [%p] at %s:%d born at %s:%d (%s)\n", stack, stack->lastUseFileName, stack->lastUseLine,
                                                                  stack->bornFileName, stack->bornLine, stack->funcPrototype);
    fprintf(outputFile, "\t{\n\tsize     = %d\n\tcapacity = %d\n\tdata[%p]:\n\t{\n", stack->size, stack->capacity, stack->data);
    if (stack->capacity > 50) {
        for (int index = 0; index < 50; index++) {
            if (index < stack->size) {
                fprintf(outputFile, "\t*[%d] = ", index);
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "\t [%d] = \n", index);
            }
        }

        fprintf(outputFile, "\t ...\n");

        for (int index = stack->capacity - 5; index < stack->capacity; index++) {
            if (index < stack->size) {
                fprintf(outputFile, "\t*[%d] = ", index);
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "\t [%d] = \n", index);
            }
        }
    }

    else {
        for (int index = 0; index < stack->capacity; index++) {
            if (index < stack->size) {
                fprintf(outputFile, "\t*[%d] = ", index);
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "\t [%d] = \n", index);
            }
        }
    }

    fprintf(outputFile, "\t}\n}\n\n");

    fclose(outputFile);

    return 0;
}

// UNDERSTAND HTML DUMP (<BR/>, <PRE>). TABULATION???

// int stackDumpHtml(stack *stack) {
//     FILE *outputFile = fopen(stack->dumpFile, "a");
//     customWarning(outputFile != NULL, 1);

//     fprintf(outputFile, "%s", startHtml);

//     fprintf(outputFile, "stack [%p] at %s:%d born at %s:%d (%s)<br/>", stack, stack->lastUseFileName, stack->lastUseLine,
//                                                                   stack->bornFileName, stack->bornLine, stack->funcPrototype);
//     fprintf(outputFile, "<pre>{<br/><pre>size     = %d<br/><pre>capacity = %d<br/><pre>data[%p]:<br/><pre>{<br/>", stack->size, stack->capacity, stack->data);
//     if (stack->capacity > 50) {
//         for (int index = 0; index < 50; index++) {
//             if (index < stack->size) {
//                 fprintf(outputFile, "*[%d] = ", index);
//                 fprintf(outputFile, "%" SPECIFICATOR_TYPE "<br/><pre>", stack->data[index]);
//             }

//             else {
//                 fprintf(outputFile, "\t [%d] = <br/>", index);
//             }
//         }

//         fprintf(outputFile, "\t ...\n");

//         for (int index = stack->capacity - 5; index < stack->capacity; index++) {
//             if (index < stack->size) {
//                 fprintf(outputFile, "\t*[%d] = ", index);
//                 fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
//             }

//             else {
//                 fprintf(outputFile, "\t [%d] = \n", index);
//             }
//         }
//     }

//     else {
//         for (int index = 0; index < stack->capacity; index++) {
//             if (index < stack->size) {
//                 fprintf(outputFile, "\t*[%d] = ", index);
//                 fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
//             }

//             else {
//                 fprintf(outputFile, "\t [%d] = \n", index);
//             }
//         }
//     }

//     fprintf(outputFile, "\t}\n}\n\n");

//     fprintf(outputFile, "%s", endHtml);

//     fclose(outputFile);

//     return 0;
// }