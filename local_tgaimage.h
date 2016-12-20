#ifndef LOCAL_TGA_IMAGE_H__
#define LOCAL_TGA_IMAGE_H__

#define TGA_FAILURE 0
#define TGA_SUCCESS 1
#define TGA_MAX_CHUNK_LEN 128

#define TGA_FOOTER "TRUEVISION-XFILE."

#define AREA_REF { 0, 0, 0, 0 }

// -----------------------------------------------------------------------------
enum TGAFormat {
      TGA_GRAYSCALE=1
    , TGA_RGB=3
    , TGA_RGBA=4
};

#pragma pack(push,1)
struct TGA_Header {
    unsigned char idlength;
    unsigned char colormaptype;
    unsigned char datatypecode;
    unsigned short colormaporigin;
    unsigned short colormaplength;
    unsigned char colormapdepth;
    unsigned short x_origin;
    unsigned short y_origin;
    unsigned short width;
    unsigned short height;
    unsigned char  bitsperpixel;
    unsigned char  imagedescriptor;
};
#pragma pack(pop)

// TGAColor --------------------------------------------------------------------
struct TGAColor {
    union {

        struct { unsigned char b, g, r, a; };
        unsigned char raw[4];
        unsigned int val;

    };

    unsigned int bytespp;
};

// TGAImage --------------------------------------------------------------------
struct TGAImage {
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int bytespp;
};

// TGA Functions ---------------------------------------------------------------
// static int unload_rle_data(void *out, struct TGA_Header *, struct TGAImage *);
// static int load_rle_data(void *in, struct TGA_Header *, struct TGAImage *);

#endif //__IMAGE_H__
