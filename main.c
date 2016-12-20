#include "tgaimage.h"
#include <stdio.h>
#include <stdlib.h>

void
line(int x0, int y0, int x1, int y1, TGAImage image, TGAColor color)
{
    float f;
    for (f = 0.0f; f < 1.0f; f += 0.01f) {

        int x = (int)((float)x0 * (1.0f - f) + (float)x1 * f);
        int y = (int)((float)y0 * (1.0f - f) + (float)y1 * f);
        TGAImage_set(x, y, image, color);
    }
}

int
main(/* int argc, char** argv */)
{
    TGAColor white = TGAColor_create(255, 255, 255, 255);
    TGAColor red = TGAColor_create(255, 0,   0,   255);
    TGAImage image = TGAImage_create(100, 100, TGA_RGB);

    line(13, 20, 80, 40, image, white);
    TGAImage_flip_vertically(image);
    write_tga_file(image, "output.tga");

    TGAColor_destroy(red);
    TGAColor_destroy(white);
    TGAImage_destroy(image);
    exit (EXIT_SUCCESS);
}
