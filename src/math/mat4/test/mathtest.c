/* mathtest.c */
#include "testmat4array.h"
#include "../include/vecMat4.h"
#include <stdlib.h>
#include <unistd.h>

float
ranf()
{
    int r = rand() % 10;
    return (float)(r + 1);
}

int
main()
{
    const float A[16] = {
           8,  3,  5,  8
        ,  1,  3,  6,  6
        ,  9,  8, 10,  1
        ,  5, 10, 10,  3
    };
    const float B[16] = {
           6,  3,  8,  7
        ,  2, 10,  9,  7
        ,  6,  4,  6,  2
        ,  8,  6,  3,  7
    };
    float a[16];
    int i = 0;
    for (; i < 16; ++i)
        a[i] = A[i];
    /*
    srand((unsigned)getpid());
    float a[16] = {
          ranf(), ranf(), ranf(), ranf()
        , ranf(), ranf(), ranf(), ranf()
        , ranf(), ranf(), ranf(), ranf()
        , ranf(), ranf(), ranf(), ranf()
    };
    float b[16] = {
          ranf(), ranf(), ranf(), ranf()
        , ranf(), ranf(), ranf(), ranf()
        , ranf(), ranf(), ranf(), ranf()
        , ranf(), ranf(), ranf(), ranf()
    };
    */
    mat4array_print(a);
    mat4array_print(B);
    mat4array_get_product(a, B);
    mat4array_print(a);
}
