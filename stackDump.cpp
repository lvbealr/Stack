#include <cstdio>
#include <cstdint>

#include "stackDump.h"
#include "customWarning/customWarning.h"

const char *startHtml = R"(<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport"
                           content="width=device-width, initial-scale=1.0"><title>Document</title></head><body>)";
const char *endHtml   = R"(</body></html>)";

#define SPECIFICATOR_TYPE "d"

// TODO DO BACKLIGHT IN DEPENDENCE OF PUSHING OR POPPING ELEMENT

#ifndef _NDEBUG

size_t previousSize = 0;

int stackDumpHtml(stack *stack) {
    FILE *outputFile = fopen(stack->dumpFile, "a");
    customWarning(outputFile != NULL, 1);

    fprintf(outputFile, "%s<pre>", startHtml);
    fprintf(outputFile, "<b><u>stack</u><font color = 'DeepSkyBlue'>" "[%p]" "</font></b>"
                        " at <b><u>%s:%d</u> <u>(%s)</u></b> <font color = 'DarkOrange'><b>born at</b></font>"
                        " <b><u>%s:%d</u></b> (%s)\n",
                        stack, stack->lastUseFileName, stack->lastUseLine, stack->lastUseFuncPrototype,
                        stack->bornFileName, stack->bornLine, stack->bornFuncPrototype);
    fprintf(outputFile, "\t{\n");
    fprintf(outputFile, "\tLEFT_STACK_INFO_CANARY [%p] = %d\n", &(stack->leftCanary), stack->leftCanary);
    fprintf(outputFile,
    "\t<b>size</b>     = <b>%lu</b>\n"
    "\t<b>capacity</b> = <b>%lu</b>\n", stack->size, stack->capacity);
    fprintf(outputFile, "\tRIGHT_STACK_INFO_CANARY [%p] = %d\n", &(stack->rightCanary), stack->rightCanary);
    fprintf(outputFile,
    "\t<b>data</b><b><font color = 'DeepSkyBlue'>" "[%p]</font></b>:\n"
    "\t{\n",stack->data);

    if (stack->memoryChunk != NULL) {
        fprintf(outputFile, "<b><font color = 'DeepPink'>\t" "LEFT_DATA_CANARY [%p]</b></font> = %d\n", stack->memoryChunk, *(stack_t *)(stack->memoryChunk));
    }

    if (stack->capacity > 50) {

        for (size_t index = 0; index < 50; index++) {

            if (index < stack->size) {
                fprintf(outputFile, "<b><font color = 'DeepPink'>\t*" "[%lu, %p]" "</b></font> = ", index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "<b><font color = 'Black'>\t " "[%lu, %p]" "</b></font> = ", index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "<font color = 'Red'><b> [POISON]</b></font>\n", stack->data[index]);
            }
        }

        fprintf(outputFile, "\t ...\n");

        for (size_t index = stack->capacity - 5; index < stack->capacity; index++) {

            if (index < stack->size) {
                fprintf(outputFile, "<b><font color = 'DeepPink'>\t*" "[%lu, %p]" "</b></font> = ", index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "<b><font color = 'Black'>\t " "[%lu, %p]" "</b></font> = ", index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "<font color = 'Red'><b> [POISON]</b></font>\n", stack->data[index]);
            }
        }
    }

    else {
        for (size_t index = 0; index < stack->capacity; index++) {

            if (index < stack->size) {
                fprintf(outputFile, "<b><font color = 'DeepPink'>\t*" "[%lu, %p]" "</b></font> = ", index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "\n", stack->data[index]);
            }

            else {
                fprintf(outputFile, "<b><font color = 'Black'>\t " "[%lu, %p]" "</b></font> = ", index, &(stack->data[index]));
                fprintf(outputFile, "%" SPECIFICATOR_TYPE "<font color = 'Red'><b> [POISON]</b></font>\n", stack->data[index]);
            }
        }
    }

    if (stack->memoryChunk != NULL) {
        fprintf(outputFile, "<b><font color = 'DeepPink'>\t" "RIGHT_DATA_CANARY [%p]</b></font> = %d\n", (stack_t *)stack->memoryChunk + stack->capacity + sizeof(CANARY), *(stack_t *)(stack->memoryChunk + stack->capacity + sizeof(CANARY)));
    }

    fprintf(outputFile, "\t}\n}\n\n");

    fprintf(outputFile, "%s", endHtml);

    fclose(outputFile);

    return 0;
}

#endif