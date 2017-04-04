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
#include <alloca.h>

#include <libdrm/drm.h> /* direct rendering infastructure */
#include <libdrm/drm_mode.h> /* direct rendering infastructure */

#include "drm_util.h"
#include "local_drm_write.h" // "types.h" -> <stdint.h>

#define DRI0_PATH "/dev/dri/card0"
#define DRI1_PATH "/dev/dri/card1"

/* DRM INFO */

int
drm_version_info(void)
{
    fd_t dri = open(DRI0_PATH, O_RDWR | O_CLOEXEC);

    if (dri < 0) {
        drm_err("Failed to open card at /dev/dir/card0\n");
        return EXIT_FAILURE;
    }

    // struct drm_version dv;
    struct all_drm_info_t *info = alloca(sizeof(struct all_drm_info_t));
    if (!info) {
        drm_err("unable to allocate all_drm_info_t\n");
        return EXIT_FAILURE;
    }

    ioctl(dri, DRM_IOCTL_VERSION, &info->dv);
    _drm_print_version_info(&info->dv);

    /*
    ioctl(dri, DRM_IOCTL_MODE_GET, &info->dmm);
    drm_print_modeinfo(&info->dmm);
    */

    return EXIT_SUCCESS;
}

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
    drm_msg("drm_render\n");

    /* open dri device */
    fd_t dri = open(DRI0_PATH, O_RDWR | O_CLOEXEC);
    if (dri < 0) {
        drm_err("Failed to open card at /dev/dir/card0\n");
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
            drm_err("Unable to map framebuffer base\n");
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
            drm_err("fb_base alloc missing\n");
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

    return drm_map(dri, create_dumb->size, (off_t)map_dumb->offset);
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
/* eof */

