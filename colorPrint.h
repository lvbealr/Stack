#ifndef IN_OUT_STREAM_H_
#define IN_OUT_STREAM_H_

enum textAttribute {
    normal      = 0,
    bold        = 1,
    underlined  = 4,
    flashing    = 5,
    inverted    = 7,
    invisible   = 8
} ; 

enum textColor {
    black       = 30,
    red         = 31,
    green       = 32,
    yellow      = 33,
    blue        = 34,
    purple      = 35,
    lightblue   = 36,
    white       = 37
};

enum backgroundColor {
    bgBlack     = 40,
    bgRed       = 41,
    bgGreen     = 42,
    bgYellow    = 43,
    bgBlue      = 44,
    bgPurple    = 45,
    bgLightblue = 46,
    bgWhite     = 47,
    bgDefault   = 48
};

void customPrint(textColor textColor, textAttribute textAttribute,
                 backgroundColor backgroundColor, const char *format, ...);

#endif