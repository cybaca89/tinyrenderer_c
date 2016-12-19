#ifndef _TGA_IMAGE_H__
#define _TGA_IMAGE_H__

// #include <fstream>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0)

// -----------------------------------------------------------------------------
enum Format {
      GRAYSCALE=1
    , RGB=3
    , RGBA=4
};

#pragma pack(push,1)
struct TGA_Header {
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack(pop)

// TGAColor --------------------------------------------------------------------
struct TGAColor {
    union {

        struct { unsigned char b, g, r, a; };
        unsigned char raw[4];
        unsigned int val;

    };

    int bytespp;
};

// extern void *TGAColor_init(unsigned int val, int bytespp);
extern void *read_tga_file(const char *filename);
extern int write_tga_file(const char *filename);
// extern int flip_horizontally(void);
// extern int flip_vertically(void);
// extern int scale(int w, int h);
// extern void *get_TGAColor(int x, int y);
// extern int set_TGAColor(int x, int y, TGAColor *c);


// TGAImage --------------------------------------------------------------------
struct TGAImage {
    unsigned char *data;
    int width;
    int height;
    int bytespp;
};

// extern int get_image_width(void);
// extern int get_image_height(void);
// extern int get_image_bytespp(void);
// extern unsigned char *buffer(void);
// extern void image_clear(void);

#endif //__IMAGE_H__
