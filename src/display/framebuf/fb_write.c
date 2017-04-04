/** @file   fb_write.c
 *  @author Cy Baca
 *
 *  @brief Low level screen rendering using the hardware framebuffer, with help
 *         from http://betteros.org/tut/graphics1.php
 *  NOTE: Look at linux_kernal/Documentation/fb/framebuffer.txt
 */
#include <stdlib.h>
#include <unistd.h>   // getpid()
#include <fcntl.h>    // open() open file descriptors
#include <sys/stat.h> // open()
#include <stropts.h>  // oictl();
// #include <assert.h>
// #include <alloca.h>

#include <linux/fb.h> // struct fb_fix_screeninfo; struct fb_var_screeninfo;
#include <linux/kd.h> // tty to graphics only mode

// #include <libdrm/drm.h> /* direct rendering infastructure */
// #include <libdrm/drm_mode.h> /* direct rendering infastructure */

#include "fb_util.h"
#include "local_fb_write.h" // "types.h" -> <stdint.h>
#include "input.h"

#define FB0_PATH  "/dev/fb0"
#define FB1_PATH  "/dev/fb1"
#define TTY0_PATH "/dev/tty0"
#define TTY1_PATH "/dev/tty1"
#define TTY2_PATH "/dev/tty2"

extern void *key_info();

struct colors_t  { uint32_t r_off, g_off, b_off; byte r, g, b; };
struct drawing_t { size_t xstep, xoffset, yoffset, linelen, xres, yres; };
struct key_in_t  { struct timeval t; uint16_t type, code; uint32_t val; };

void
function(void)
/* set tty to graphics only */
{
    fd_t tty_fd = open(TTY2_PATH, O_RDWR);
    ioctl(tty_fd, KDSETMODE, KD_GRAPHICS);
    /* ...  */
    ioctl(tty_fd, KDSETMODE, KD_TEXT);
}

static inline uint32_t
_fb_set_pixel_color(const struct colors_t *const c)
{
    return (unsigned)((c->r << c->r_off)
                    | (c->g << c->g_off)
                    | (c->b << c->b_off));
}

int
fb_clear(
const struct drawing_t *const d,
const struct colors_t  *const c,
uint8_t                *ptr)
{
    size_t i;
    for (i = 0; i < d->xres; ++i) {
        size_t j;
        for (j = 0; j < d->yres; ++j) {
            size_t loc =
                (i + d->xoffset) * d->xstep + (j + d->yoffset) * d->linelen;
            *((uint32_t *)(ptr + loc)) = _fb_set_pixel_color(c);
        }
    }
    return EXIT_SUCCESS;
}

int
fb_render(void)
#define BPB 8
{
    fb_msg("framebuf_render\n");

    /* open /dev/tty2 and set to graphics mode */
    fd_t tty_fd = open(TTY2_PATH, O_RDWR);
    if (tty_fd < 0) goto tty_open_failure;
    ioctl(tty_fd, KDSETMODE, KD_GRAPHICS);

    fd_t fb = open(FB0_PATH, O_RDWR);
    if (fb < 0) goto framebuffer_open_failure;

    fd_t kb = key_init();
    if (kb < 1) goto keyboard_init_failure;

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;

    /** use FBIOPUT_... to set changed variable info */
    ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
    size_t screensize = (size_t)vinfo.yres_virtual * (size_t)finfo.line_length;
    uint8_t *fbp = fb_map(fb, screensize, (off_t)0);
    if (!fbp) goto framebuffer_map_failure;

    struct colors_t clr = { 0 };
    struct key_in_t *ki = NULL;
    clr.r_off = vinfo.red.offset;
    clr.g_off = vinfo.green.offset;
    clr.b_off = vinfo.blue.offset;


    /** calculate screen size in bytes */
    /** find location of mapped memory */
    struct drawing_t draw = {
          (size_t)(vinfo.bits_per_pixel / BPB)
        , (size_t)vinfo.xoffset
        , (size_t)vinfo.yoffset
        , (size_t)finfo.line_length
        , (size_t)vinfo.xres
        , (size_t)vinfo.yres
    };

    ki = key_info(&kb);

    if (!ki) goto keyboard_init_failure;

    while(1) {
        key_info(&kb);
        switch (ki->code) {
            case K_Q: ioctl(tty_fd, KDSETMODE, KD_TEXT); return EXIT_SUCCESS;
            case K_H: ++clr.r; break;
            case K_J: ++clr.g; break;
            case K_K: ++clr.b; break;
            case K_L: clr.r = 0xff; clr.g = 0x00; clr.b = 0x00; break;
            default: break;
        }

        fb_clear(&draw, &clr, fbp);
    }

    ioctl(tty_fd, KDSETMODE, KD_TEXT);
    return EXIT_SUCCESS;

    framebuffer_open_failure:
        fb_err("Failed to open path at /dev/fb0/\n");
        ioctl(tty_fd, KDSETMODE, KD_TEXT);
        return EXIT_FAILURE;

    framebuffer_map_failure:
        fb_err("FRAMEBUFFER0 MAP FAILED\n");
        ioctl(tty_fd, KDSETMODE, KD_TEXT);
        return EXIT_FAILURE;

    keyboard_init_failure:
        ioctl(tty_fd, KDSETMODE, KD_TEXT);
        return EXIT_FAILURE;

    tty_open_failure:
        fb_err("tty2 open\n");
        return EXIT_FAILURE;
}

int
fb_screen_info(void)
{
    fd_t fb0 = open(FB0_PATH, O_RDWR);
    if (fb0 < 0) {
        fb_err("Failed to open path at /dev/fb0\n");
        return EXIT_FAILURE;
    }
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    ioctl(fb0, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb0, FBIOGET_FSCREENINFO, &finfo);
    fb_msg("/dev/db0:\n");
    print_fb_var_screeninfo(&vinfo);
    print_fb_fix_screeninfo(&finfo);

    fd_t fb1 = open(FB1_PATH, O_RDWR);
    if (fb1 < 0) {
        fb_err("Failed to open path at /dev/fb1\n");
        return EXIT_FAILURE;
    }
    ioctl(fb1, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb1, FBIOGET_FSCREENINFO, &finfo);
    fb_msg("/dev/db1:\n");
    print_fb_var_screeninfo(&vinfo);
    print_fb_fix_screeninfo(&finfo);

    return EXIT_SUCCESS;
}
/* eof */

