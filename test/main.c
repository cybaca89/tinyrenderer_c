#include "tgaimage.h"
#include <stdio.h>
#include <stdlib.h>

// const TGAColor *white = TGAColor_create(255, 255, 255, 255);
// const TGAColor *red = TGAColor_create(255, 0,   0,   255);

int
main(/* int argc, char** argv */)
{
    struct TGAColor *red = TGAColor_create(255, 0, 0, 255);

    struct TGAImage *image = TGAImage_create(100, 100, TGA_RGB);
    TGAImage_set(52, 41, image, red);
    TGAImage_flip_vertically(image);
    write_tga_file(image, "output.tga");
    // TGAImage image(100, 100, TGAImage::RGB);
    // image.set(52, 41, red);

    /* i want to have the origin at the left bottom corner of the image */
    // image.flip_vertically();
    // image.write_tga_file("output.tga");
    TGAColor_destroy(red);
    TGAImage_destroy(image);
    exit (EXIT_SUCCESS);
}
