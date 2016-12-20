#ifndef _TGA_IMAGE_H__
#define _TGA_IMAGE_H__

enum TGAFormat {
      TGA_GRAYSCALE=1
    , TGA_RGB=3
    , TGA_RGBA=4
};

// TGAColor --------------------------------------------------------------------
struct TGAColor {
    union {

        struct { unsigned char r, g, b, a; };
        unsigned char raw[4];
        unsigned int val;

    };

    unsigned int bytespp;
};

extern void *TGAColor_create(
  const unsigned char r
, const unsigned char g
, const unsigned char b
, const unsigned char a
);

extern void TGAColor_destroy(struct TGAColor *);

// TGAImage --------------------------------------------------------------------
struct TGAImage {
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int bytespp;
};

// extern void *read_tga_file(const char *filename);
extern int write_tga_file(struct TGAImage *, const char *filepath);
extern void *TGAImage_create(int w, int h, int bpp);
extern int TGAImage_set(int x, int y, struct TGAImage *i, struct TGAColor *c);
extern int TGAImage_flip_vertically(struct TGAImage *image);
extern void TGAImage_destroy(struct TGAImage *image);

typedef struct TGAImage * TGAImage;
typedef struct TGAColor * TGAColor;

#endif
