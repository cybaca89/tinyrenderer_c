/** @file   fb_write.c
 *  @author Cy Baca
 *
 *  @brief Low level screen rendering using the hardware framebuffer, with help
 *         from http://betteros.org/tut/graphics1.php
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

#include <libdrm/drm.h> /* direct rendering infastructure */
#include <libdrm/drm_mode.h> /* direct rendering infastructure */

#include "util.h"
#include "local_framebuffer_write.h" // "types.h" -> <stdint.h>

#define FB0_PATH  "/dev/fb0"
#define TTY0_PATH "/dev/tty1"
#define DRI0_PATH "/dev/dri/card0"

/* DRM -----------------------------------------------------------------------*/
/* kernel mode setting (kms) */
static void
_drm_get_kernel_mode_setting(
const fd_t dri,
const uint32_t encoder_id,
const uint32_t cmd_fb_id,
const uint64_t conn_buf,
struct drm_mode_crtc *crtc,
struct drm_mode_get_encoder *get_enc,
struct drm_mode_modeinfo *mode
)
{
    get_enc->encoder_id = encoder_id;

    ioctl(dri, DRM_IOCTL_MODE_GETENCODER, get_enc);

    crtc->crtc_id = get_enc->crtc_id;

    ioctl(dri, DRM_IOCTL_MODE_GETCRTC, crtc);

    crtc->fb_id = cmd_fb_id;
    crtc->set_connectors_ptr = conn_buf;// (uint64_t)&res_buf.conn[i];
    crtc->count_connectors   = 1;
    crtc->mode               = *mode; // modebuf.mode[0];
    crtc->mode_valid         = 1;

    ioctl(dri, DRM_IOCTL_MODE_SETCRTC, crtc);
}

static void
_drm_write_stuff(
void *fb_base[BUF_LEN],
const size_t fb_w[BUF_LEN],
const size_t fb_h[BUF_LEN],
const int count_conn)
#define SLEEPTIME 100000
{
    srand((unsigned)getpid());
    int i;
    for (i = 0; i < 100; ++i) {

        int j;
        for (j = 0; j < count_conn; ++j) {

            uint32_t col = (rand() % 0x00ffffff)&0x00ff00ff;

            size_t y;
            for (y = 0; y < fb_h[j]; ++y) {

                size_t x;
                for (x = 0; x < fb_w[j]; ++x) {
                    size_t loc = y * fb_w[j] + x;
                    *(((uint32_t *)fb_base[j]) + loc) = col;
                }
            }
        }
        usleep(SLEEPTIME);
    }
}

/** DRM_IOCTL_SET_MASTER
 *  DRM_IOCTL_MODE_GETRESOURCES
 *  DRM_IOCTL_MODE_GETRESOURCES
 *  DRM_IOCTL_MODE_GETCONNECTOR  count_modes, enc_id returning 0
 *  DRM_IOCTL_MODE_GETCONNECTOR
 *  DRM_IOCTL_MODE_CREATE_DUMB
 *  DRM_IOCTL_MODE_ADDFB
 *  DRM_IOCTL_MODE_MAP_DUMB
 *  DRM_IOCTL_MODE_GETENCODER
 *  DRM_IOCTL_MODE_GETCRTC
 *  DRM_IOCTL_MODE_GETCRTC
 */
int
drm_render(void)
{
    print_msg("drm_render\n");

    /* open dri device */
    fd_t dri = open(DRI0_PATH, O_RDWR | O_CLOEXEC);
    if (dri < 0) {
        print_err("Failed to open card at /dev/dir/card0\n");
        return EXIT_FAILURE;
    }

    /* become master of dri device */
    ioctl(dri, DRM_IOCTL_SET_MASTER, 0);

    /* buffers for drm card info */
    struct drm_mode_card_res card_res = { 0 };
    struct res_buf_t          res_buf = { 0 };
    _drm_get_card_info(dri, &card_res, &res_buf);

    /* print info to console wtfn */
    print_cardres_counts(&card_res);

    /* dumb framebuffer */
    void   *fb_base[BUF_LEN];
    size_t  fb_w[BUF_LEN];
    size_t  fb_h[BUF_LEN];

    /* kms setup loop */
    uint32_t count_conn = card_res.count_connectors, i;
    for (i = 0; i < count_conn; ++i) {

        /* mode info */
        struct conn_modebuf_t         modebuf  = { 0 };
        struct drm_mode_get_connector get_conn = { 0 };

        if (_drm_get_conn_resources(
                dri, &get_conn, &modebuf, &res_buf.conn[i]))
            continue;

        /* instantiate dumb buffer data structures */
        struct drm_mode_create_dumb create_dumb = { 0 };
        struct drm_mode_map_dumb       map_dumb = { 0 };
        struct drm_mode_fb_cmd         cmd_dumb = { 0 };
        fb_base[i] = _drm_init_dumb_buffer(dri
            , &modebuf.mode[0]
            , &create_dumb
            , &map_dumb
            , &cmd_dumb);

        if (!fb_base[i]) {
            print_err("Unable to map framebuffer base\n");
            return EXIT_FAILURE;
        }

        fb_w[i]    = create_dumb.width;
        fb_h[i]    = create_dumb.height;

        /* print info */
        print_conn_and_mode_info(&get_conn, &modebuf, (uint64_t)fb_base[i]);

        /* encoders */
        struct drm_mode_crtc           crtc = { 0 };
        struct drm_mode_get_encoder get_enc = { 0 };

        /* kernel mode setting (kms) */
        _drm_get_kernel_mode_setting(
              dri
            , get_conn.encoder_id
            , cmd_dumb.fb_id
            , (uint64_t)&res_buf.conn[i]
            , &crtc
            , &get_enc
            , &modebuf.mode[0]);
    }

    /* no longer need master */
    ioctl(dri, DRM_IOCTL_DROP_MASTER, 0);

    for (i = 0; i < BUF_LEN; ++i) {
        if (!fb_base[i]) {
            print_err("fb_base alloc missing\n");
            return EXIT_FAILURE;
        }
    }

    /* draw on our fb using drm driver to deliver data to screen */
    _drm_write_stuff(fb_base, fb_w, fb_h, (int)count_conn);

    return EXIT_SUCCESS;
}

static void
_drm_get_card_info(
    const fd_t                dri,
    struct drm_mode_card_res *card_res,
    struct res_buf_t         *res_buf)
{

    /* get buffer lengths */
    ioctl(dri, DRM_IOCTL_MODE_GETRESOURCES, card_res);

    card_res->fb_id_ptr        = (uint64_t)res_buf->fbuf;
    card_res->crtc_id_ptr      = (uint64_t)res_buf->crtc;
    card_res->connector_id_ptr = (uint64_t)res_buf->conn;
    card_res->encoder_id_ptr   = (uint64_t)res_buf->enco;

    /* fill buffers with ptrs to info */
    ioctl(dri, DRM_IOCTL_MODE_GETRESOURCES, card_res);
}

static void *
_drm_init_dumb_buffer(
const fd_t                   dri,
struct drm_mode_modeinfo    *conn_mode_buf,
struct drm_mode_create_dumb *create_dumb,
struct drm_mode_map_dumb    *map_dumb,
struct drm_mode_fb_cmd      *cmd_dumb)
{
    /* fill out create_dumb with a mode, call ioctl to fill handles */
    create_dumb->width  = conn_mode_buf->hdisplay;
    create_dumb->height = conn_mode_buf->vdisplay;
    create_dumb->bpp    = 32;
    create_dumb->flags  = 0;
    create_dumb->pitch  = 0;
    create_dumb->size   = 0;
    create_dumb->handle = 0;
    ioctl(dri, DRM_IOCTL_MODE_CREATE_DUMB, create_dumb);

    /* put handle into the drm_mode_fb_cmd structure */
    cmd_dumb->width  = create_dumb->width;
    cmd_dumb->height = create_dumb->height;
    cmd_dumb->bpp    = create_dumb->bpp;
    cmd_dumb->pitch  = create_dumb->pitch;
    cmd_dumb->depth  = 24;
    cmd_dumb->handle = create_dumb->handle;
    ioctl(dri, DRM_IOCTL_MODE_ADDFB, cmd_dumb);

    /* prepare framebuffer for mapping */
    map_dumb->handle = create_dumb->handle;
    ioctl(dri, DRM_IOCTL_MODE_MAP_DUMB, map_dumb);

    return fb_map(dri, create_dumb->size, (off_t)map_dumb->offset);
}

#include <stdio.h>
static int
_drm_get_conn_resources(
const fd_t                     dri,
struct drm_mode_get_connector *get_conn,
struct conn_modebuf_t         *modebuf,
uint64_t                      *conn)
{
    get_conn->connector_id = *(uint32_t *)conn;

    /* get connector resource counts */
    ioctl(dri, DRM_IOCTL_MODE_GETCONNECTOR, get_conn);

    /* get connector resources */
    get_conn->modes_ptr       = (uint64_t)modebuf->mode;
    get_conn->props_ptr       = (uint64_t)modebuf->prop;
    get_conn->prop_values_ptr = (uint64_t)modebuf->propval;
    get_conn->encoders_ptr    = (uint64_t)modebuf->enc;

    ioctl(dri, DRM_IOCTL_MODE_GETCONNECTOR, get_conn);

    printf(
    "count_encoders: %u\n"
    "count_modes:    %u\n"
    "encoder_id:     %u\n"
    "connection:     %u\n"
    , get_conn->count_encoders
    , get_conn->count_modes
    , get_conn->encoder_id
    , get_conn->connection);

    /* if connector isnt valid, reenter loop */
    return get_conn->count_encoders < 1
        ||    get_conn->count_modes < 1
        ||   !get_conn->encoder_id
        ||   !get_conn->connection;
}

/* RENDER TO FB0 -------------------------------------------------------------*/
void
function(void)
/* set tty to graphics only */
{
    fd_t tty_fd = open("/dev/tty1", O_RDWR);
    ioctl(tty_fd, KDSETMODE, KD_GRAPHICS);
    /* ... */
    ioctl(tty_fd, KDSETMODE, KD_TEXT);
}

static inline uint32_t
_fb_set_pixel_color(const byte r, const byte g, const byte b,
const struct fb_var_screeninfo *const v)
{
    return (unsigned)((r << v->red.offset  )
                    | (g << v->green.offset)
                    | (b << v->blue.offset ));
}

int
framebuf_render(void)
#define BPB 8
{
    int
    _framebuf_render(
    const struct fb_var_screeninfo *const v,
    const struct fb_fix_screeninfo *const f,
    const fd_t fb)
    {
        print_msg("framebuf_render\n");
        /** calculate screen size in bytes */
        size_t screensize = (size_t)v->yres_virtual * (size_t)f->line_length;
        // uint8_t *fbp = NULL;
        // fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb,
        //     (off_t)0);
        uint8_t *fbp = fb_map(fb, screensize, (off_t)0);
        if (!fbp) {
            print_err("FRAMEBUFFER0 MAP FAILED\n");
            return EXIT_FAILURE;
        }

        /** find location of mapped memory */
        size_t xstep   = (size_t)(v->bits_per_pixel / BPB);
        size_t xoffset = (size_t)v->xoffset;
        size_t yoffset = (size_t)v->yoffset;
        size_t linelen = (size_t)f->line_length;

        const size_t xres = v->xres;
        const size_t yres = v->yres;
        size_t i;
        for (i = 0; i < xres; ++i) {
            size_t j;
            for (j = 0; j < yres; ++j) {
                size_t loc = (i + xoffset) * xstep + (j + yoffset) * linelen;
                *((uint32_t *)(fbp + loc)) = _fb_set_pixel_color(
                    0xff, 0x00, 0xff, v);
            }
        }
        pause();
        return EXIT_SUCCESS;
    }

    fd_t fb = open(FB0_PATH, O_RDWR);
    if (fb < 0) {
        print_err("Failed to open path at /dev/fb0/\n");
        return EXIT_FAILURE;
    }
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;

    /** use FBIOPUT_... to set changed variable info */
    ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb, FBIOGET_FSCREENINFO, &finfo);

    return _framebuf_render(&vinfo, &finfo, fb);
}

void
fb_screen_info(void)
{
    fd_t fb = open(FB0_PATH, O_RDWR);
    if (fb < 0) {
        print_err("Failed to open path at /dev/fb0\n");
        exit (EXIT_FAILURE);
    }
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb, FBIOGET_FSCREENINFO, &finfo);
    print_fb_var_screeninfo(&vinfo);
    print_fb_fix_screeninfo(&finfo);
}
/* eof */

