#include <cstdio>
#include <cstdint>

#include "stackDump.h"
#include "customExits.h"

const char *startHtml = R"(<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport"
                           content="width=device-width, initial-scale=1.0"><title>Document</title></head><body bgcolor = 'Beige'>)";
const char *endHtml   = R"(</body></html>)";

#define SPECIFICATOR_TYPE "d"

int stackDumpHtml(stack *stack) {
    FILE *outputFile = fopen(stack->dumpFile, "a");

    if (outputFile == NULL) {
        return 1;
    }

    fprintf(outputFile, "%s<pre>", startHtml);
    fprintf(outputFile, "<b><u>stack</u><font color = 'DeepSkyBlue'>" "[%p]" "</font></b>"
                        " at <b><u>%s:%d</u> <u>(%s)</u></b> <font color = 'DarkOrange'><b>born at</b></font>"
                        " <b><u>%s:%d</u></b> (%s)\n",
                        stack, stack->lastUseFileName, stack->lastUseLine, stack->lastUseFuncPrototype,
                        stack->bornFileName, stack->bornLine, stack->bornFuncPrototype);
    fprintf(outputFile, "\t{\n");
    #ifndef _NDEBUG
    fprintf(outputFile, "\tLEFT_STACK_INFO_CANARY [%p] = %d\n", &(stack->leftCanary), stack->leftCanary);
    #endif
    fprintf(outputFile,
    "\t<b>size</b>     = <b>%d</b>\n"
    "\t<b>capacity</b> = <b>%d</b>\n", stack->size, stack->capacity);
    #ifndef _NDEBUG
    fprintf(outputFile, "\tRIGHT_STACK_INFO_CANARY [%p] = %d\n", &(stack->rightCanary), stack->rightCanary);
    #endif
    fprintf(outputFile,
    "\t<b>data</b><b><font color = 'DeepSkyBlue'>" "[%p]</font></b>:\n"
    "\t{\n",stack->data);

    if (stack->memoryChunk != NULL) {
        fprintf(outputFile, "<b><font color = 'DeepPink'>\t" "LEFT_DATA_CANARY [%p]</b></font> = %d\n",
                stack->memoryChunk, *(stack_t *)(stack->memoryChunk));
    }

    if (stack->capacity > 50) {

        for (int index = 0; index < 50; index++) {

            if (index < stack->size) {
                fprintf(outputFile, "<b><font color = 'DeepPink'>\t*" "[%d, %p]" "</b></font> = ",
                        index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "<b><font color = 'Black'>\t " "[%d, %p]" "</b></font> = ",
                        index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "<font color = 'Red'><b> [POISON]</b></font>\n",
                        stack->data[index]);
            }
        }

        fprintf(outputFile, "\t ...\n");

        for (int index = stack->capacity - 5; index < stack->capacity; index++) {

            if (index < stack->size) {
                fprintf(outputFile, "<b><font color = 'DeepPink'>\t*" "[%d, %p]" "</b></font> = ",
                        index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "<b><font color = 'Black'>\t " "[%d, %p]" "</b></font> = ",
                        index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "<font color = 'Red'><b> [POISON]</b></font>\n",
                        stack->data[index]);
            }
        }
    }

    else {
        for (int index = 0; index < stack->capacity; index++) {

            if (index < stack->size) {
                fprintf(outputFile, "<b><font color = 'DeepPink'>\t*" "[%d, %p]" "</b></font> = ",
                        index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "<b><font color = 'Black'>\t " "[%d, %p]" "</b></font> = ",
                        index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "<font color = 'Red'><b> [POISON]</b></font>\n",
                        stack->data[index]);
            }
        }
    }

    if (stack->memoryChunk != NULL) {
        fprintf(outputFile, "<b><font color = 'DeepPink'>\t" "RIGHT_DATA_CANARY [%p]</b></font> = %d\n",
                (stack_t *)stack->memoryChunk + stack->capacity + 1,
                         *(stack->memoryChunk + stack->capacity + 1));
    }

    #ifndef _NDEBUG
    fprintf(outputFile, "\t<b><font color = 'Maroon'>STACK_HASH_VALUE = </font>%lu</b>\n", stack->hash);
    #endif
    fprintf(outputFile, "\t}\n}\n\n");

    fprintf(outputFile, "%s", endHtml);

    fclose(outputFile);

    return 0;
}