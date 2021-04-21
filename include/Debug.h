#ifndef NDEBUG
    #include <stdio.h>
    #define DEBUG(f_, ...) printf((f_), ##__VA_ARGS__)
#else
    #define DEBUG(f_, ...)
#endif