#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "tgaimage.h"
// -----------------------------------------------------------------------------

int
write_tga_file(const char *filepath /* , bool rle */)
{
    unsigned char developer_area_ref[4] = { 0, 0, 0, 0 };
    unsigned char extension_area_ref[4] = { 0, 0, 0, 0 };
    /*
    unsigned char footer[18] = {
         'T','R','U','E'
        ,'V','I','S','I','O','N'
        ,'-','X','F','I','L','E'
        ,'.','\0'
    };
    */
    const char *footer[] = { "TRUEVISION-XFILE." };

    FILE *fp = fopen(filename, "w+");

    if (!fp) {
        handle_error("Unable to open specified file.\n");
        return 0;
    }

    TGA_Header header;

    memset((void 

}

void *
read_tga_file(const char *filepath)
{
    FILE *fp = fopen(filepath, "r");

    if (!fp) {
        fprintf(stderr, "Failed to open file at \"%s\"\n", filepath);
        goto tga_read_error;
    }

    // TGA_Header header;

    fclose(fp);

tga_read_error:
    return NULL;
}

/*
void *
TGAColor_init(unsigned char *p, int bpp)
{
    TGAColor *tga_color = malloc(sizeof(tga_color));
    TGAColor->val = 0;
    TGAColor->bytecpp = bpp;
    int i;
    for (i = 0; i < bpp; ++i)

        raw[i] = p[i];

}
*/
