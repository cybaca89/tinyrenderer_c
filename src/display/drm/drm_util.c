#include <libdrm/drm_mode.h>
#include <linux/fb.h>
#include <stdint.h>
#include <assert.h>
#include "drm_types.h"

/* MEM          --------------------------------------------------------------*/
#include <sys/mman.h> // mmap() for accessing framebuffer space to write to
void *
drm_map(const fd_t fd, const size_t bytes, const off_t offset)
{
    assert(fd > 2);
    assert(bytes > 0);
    return mmap(0, bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
}

/* IO <stdio.h> --------------------------------------------------------------*/
#include <stdio.h>

void
drm_print_modeinfo(const struct drm_mode_modeinfo *dmm)
{
    printf(
    "clock:       %u\n"
    "hdisplay:    %u\n"
    "hsync_start: %u\n"
    "hsync_end:   %u\n"
    "htotal:      %u\n"
    "hskew:       %u\n"
    "vdisplay:    %u\n"
    "vsync_start: %u\n"
    "vsync_end:   %u\n"
    "vtotal:      %u\n"
    "vscan:       %u\n"
    "vrefresh:    %u\n"
    "flags:       %u\n"
    "type:        %u\n"
    "name:        %s\n"
    , dmm->clock
    , dmm->hdisplay
    , dmm->hsync_start
    , dmm->hsync_end
    , dmm->htotal
    , dmm->hskew
    , dmm->vdisplay
    , dmm->vsync_start
    , dmm->vsync_end
    , dmm->vtotal
    , dmm->vscan
    , dmm->vrefresh
    , dmm->flags
    , dmm->type
    , dmm->name);
}

void
_drm_print_version_info(const struct drm_version *const dv)
{
    printf(
    "Major version: %i\n"
    "Minor version: %i\n"
    "version patch: %i\n"
    "driver name:   %s\n"
    , dv->version_major
    , dv->version_minor
    , dv->version_patchlevel
    , dv->name);
}

void
print_cardres_counts(const struct drm_mode_card_res *const card_res)
{
    assert(card_res);
    printf(
    "framebuffers:                         %u\n"
    "crtcs(cathode ray tube controllers?): %u\n"
    "connectors(e.g. vga, htmi, etc):      %u\n"
    "encoders:                             %u\n\n\n"
    , card_res->count_fbs
    , card_res->count_crtcs
    , card_res->count_connectors
    , card_res->count_encoders);
}

void
print_conn_and_mode_info(
const struct drm_mode_get_connector *const get_conn,
const struct conn_modebuf_t         *const modebuf,
const uint64_t fb_base
)
{
    assert(get_conn);
    assert(modebuf);
    printf(
    "%u:\n"
    "mode: %u\n"
    "prop: %u\n"
    "enc:  %u\n\n\n"

    "modes: %ux%u\n"
    "FB:    %lu\n\n\n"
    , get_conn->connection
    , get_conn->count_modes
    , get_conn->count_props
    , get_conn->count_encoders

    , modebuf->mode[0].hdisplay
    , modebuf->mode[0].vdisplay
    , fb_base);
}

void
drm_err(const char *msg) {
    assert(msg);
    fprintf(stderr, msg);
}

void
drm_msg(const char *msg) {
    assert(msg);
    printf(msg);
}
/* eof */

