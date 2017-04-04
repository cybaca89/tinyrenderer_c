#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "local_tgaimage.h"
// -----------------------------------------------------------------------------

static int
unload_rle_data(
  FILE *out
, struct TGA_Header *header
, struct TGAImage *image)
{
    size_t num_pixels = (size_t)(header->width * header->height);
    size_t current_pixel = 0;
    size_t bytespp = (size_t)image->bytespp;
    const unsigned char *data = image->data;

    while(current_pixel < num_pixels) {

        size_t chunk_start = current_pixel * bytespp;
        size_t current_byte = current_pixel * bytespp;
        unsigned char run_length = 1;
        int raw = 1;

        while (current_pixel + (size_t)run_length
               < num_pixels && run_length < TGA_MAX_CHUNK_LEN) {

            size_t succ_eq = 1;
            size_t i;
            for (i = 0; succ_eq && i < bytespp; ++i)

                succ_eq = (
                    data[current_byte + i] ==
                    data[current_byte + i + bytespp]
                );

            current_byte += bytespp;

            if (1 == run_length) raw = !succ_eq;
            if (raw && succ_eq) { --run_length; break; }
            if (!raw && !succ_eq) break;
            ++run_length;
        }

        current_pixel += run_length;

        if (fputc(raw ? run_length - 1 : run_length + 127, out) == EOF)
            return TGA_FAILURE;

        if (!fwrite(
              (unsigned char *)(data + chunk_start)
            , raw ? run_length * bytespp
                  : bytespp
            , 1
            , out
            ))
            return TGA_FAILURE;
    }

    return TGA_SUCCESS;
}

/*
static int
load_rle_data(
  FILE *in
, struct TGA_Header *header
, struct TGAImage *data)
{
    unsigned long pixelcount = (unsigned long)(data->width * data->height);
    unsigned long currentpixel = 0;
    unsigned long currentbyte = 0;
    TGAColor colorbuffer;

    do {

        unsigned char chunkheader = 0;
        chunkheader = fgetc(in);

        if (!chunkheader)
            return TGA_FAIL;

        if (chunkheader < 128) {
            ++chunkheader;
            int i;
            for (i = 0; i < chunkheader; ++i) {

                if (!fread(
                      colorbuffer.raw
                    , data->bytespp
                    , 1
                    , in))
                    return TGA_FAIL;

                int j;
                for (j = 0; j < data->bytescpp; ++j)

                    data[currentbyte++] = colorbuffer.raw[j];

                if (++currentpixel > pixelcount)

                    return TGA_FAIL;
            }
        } else {
            chunkheader -= 127;
            if (!fread(
                  colorbuffer.raw
                , data->bytespp
                , 1
                , in))
                return TGA_FAIL;

            int i;
            for (i = 0; i < chunkheader; ++i) {
                int j;
                for (j = 0; j < data->bytespp; ++j)
                    data[currentbyte++] = colorbuffer.raw[j];
                if (++currentpixel > pixelcount) {
                    return TGA_FAIL;
                }
            }
        }
    } while(currentpixel < pixelcount);

    return TGA_SUCCESS;
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
*/

int
write_tga_file(struct TGAImage *image, const char *filepath /* , bool rle */)
{
    unsigned char developer_area_ref[4] = AREA_REF;
    unsigned char extension_area_ref[4] = AREA_REF;

    FILE *fp = fopen(filepath, "w+");

    if (!fp) {
        fprintf(stderr, "Unable to open file \"%s\"\n", filepath);
        return TGA_FAILURE;
    }

    struct TGA_Header header;

    memset((void  *)&header, 0, sizeof(header));

    header.bitsperpixel = (unsigned char)(image->bytespp << 3);
    header.width = (unsigned short)image->width;
    header.height = (unsigned short)image->height;
    header.datatypecode =
        (unsigned short)image->bytespp == TGA_GRAYSCALE ? 11 : 10;
    header.imagedescriptor = 0x20; // top-left origin

    if (!fwrite((unsigned char *)&header, sizeof(header), 1, fp))
        goto tga_write_error;

    if (!unload_rle_data(fp, &header, image))
        goto tga_write_error;

    if (!fwrite(developer_area_ref, sizeof(developer_area_ref), 1, fp))
        goto tga_write_error;

    if (!fwrite(extension_area_ref, sizeof(developer_area_ref), 1, fp))
        goto tga_write_error;

    if (!fwrite(TGA_FOOTER, sizeof(TGA_FOOTER), 1, fp))
        goto tga_write_error;

    fclose(fp);

    return TGA_SUCCESS;

tga_write_error:
    if (fp)
        fclose(fp);
    fprintf(stderr, "Unable to dump the tga file: \"%s\"\n", filepath);
    return TGA_FAILURE;

}

/*
int
flip_horizontally()
{
}

int
flip_vertically()
{
}

int
scale(int w, int h)
{
}

void *
get_TGAColor(int x, int y)
{
}

int
set_TGAColor(int x, int y, TGAColor *c)
{
}
*/

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

void *
TGAColor_create(
  const unsigned char r
, const unsigned char g
, const unsigned char b
, const unsigned char a)
{
    struct TGAColor *color = malloc(sizeof(struct TGAColor));
    if (!color) {
        fprintf(stderr, "Failed to create item \"TGAColor\"\n");
        exit(EXIT_FAILURE);
    }

    color->b = b;
    color->g = g;
    color->r = r;
    color->a = a;

    return color;
}

void *
TGAImage_create(
  const unsigned int w
, const unsigned int h
, const unsigned int bpp)
{
    struct TGAImage *image = malloc(sizeof(struct TGAImage));

    unsigned char *data = malloc((size_t)(w * h * bpp));
    memset(data, 0, (size_t)(w * h * bpp));

    image->data = data;
    image->width = w;
    image->height = h;
    image->bytespp = bpp;

    return image;
}

int
TGAImage_set(int x, int y, struct TGAImage *i, struct TGAColor *c)
{
    if (!i->data
        || x < 0
        || y < 0
        || (unsigned)x >= i->width
        || (unsigned)y >= i->height)
        return TGA_FAILURE;

    memcpy(
          (void *)(i->data + (x + y * (int)i->width) * (int)i->bytespp)
        , c->raw
        , (size_t)i->bytespp);
    return TGA_SUCCESS;
}

int
TGAImage_flip_vertically(struct TGAImage *image)
{
    if (!image->data) return TGA_FAILURE;

    unsigned char *data = image->data;

    size_t width = (size_t)image->width;
    size_t height = (size_t)image->height;
    size_t bytespp = (size_t)image->bytespp;
    size_t bytes_per_line = width * bytespp;

    unsigned char *line = malloc(bytes_per_line);

    int half = (int)(height >> 1);

    int i;
    for (i = 0; i < half; ++i) {
        size_t l1 = (size_t)i * bytes_per_line;
        size_t l2 = (height - 1 - (size_t)i) * bytes_per_line;

        memmove((void *)line       , (void *)(data + l1), bytes_per_line);
        memmove((void *)(data + l1), (void *)(data + l2), bytes_per_line);
        memmove((void *)(data + l2), (void *)line       , bytes_per_line);
    }
    free(line);
    return TGA_SUCCESS;
}

void
TGAImage_destroy(struct TGAImage *image)
{
    if (image) {
        if (image->data) free(image->data);
        free(image);
    }
}

void
TGAColor_destroy(struct TGAColor *color)
{
    if (color)
        free(color);
}
