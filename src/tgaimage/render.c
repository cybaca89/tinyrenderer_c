#include "header/tgaimage.h"
#include <stdio.h>
//------------------------------------------------------------------------------
typedef unsigned char byte;

static void
int_swap(int *restrict x, int *restrict y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void
bresenham_line(
  int x0, int y0
, int x1, int y1
, struct TGAImage *image
, struct TGAColor *color)
{
    byte flip = (__builtin_abs(x0 - x1) < __builtin_abs(y0 - y1));

    if (flip)
        int_swap(&x0, &y0),
        int_swap(&x1, &y1);

    if (x0 > x1)
        int_swap(&x0, &x1),
        int_swap(&y0, &y1);

    const float coef = 1.0f / (float)(x1 - x0);
    if (flip) {

        int x;
        for (x = x0; x <= x1; ++x) {
            float t = (float)(x - x0) * coef;
            int y = (int)((float)y0 * (1.0f - t) + (float)y1 * t);
            TGAImage_set(y, x, image, color);
        }
    } else {

        int x;
        for (x = x0; x <= x1; ++x) {
            float t = (float)(x - x0) * coef;
            int y = (int)((float)y0 * (1.0f - t) + (float)y1 * t);
            TGAImage_set(x, y, image, color);
        }
    }
}
