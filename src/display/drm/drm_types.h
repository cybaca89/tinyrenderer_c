#ifndef DRM_TYPES_H_
#define DRM_TYPES_H_
#include <stdint.h>
struct drm_version;
struct drm_mode_modeinfo;
struct drm_mode_card_res;
struct drm_mode_crtc;

/* DRM -----------------------------------------------------------------------*/
#define NUM_CONN_BUF 20
#define BUF_LEN 10

typedef int fd_t;
typedef uint8_t byte;

struct all_drm_info_t {
    struct drm_version dv;
    struct drm_mode_modeinfo dmm;
    // struct drm_mode_card_res;
    // struct drm_mode_crtc;
};

/* mode info buffers */
struct conn_modebuf_t {
    struct drm_mode_modeinfo mode[NUM_CONN_BUF];
    uint64_t                 prop[NUM_CONN_BUF];
    uint64_t                 propval[NUM_CONN_BUF];
    uint64_t                 enc[NUM_CONN_BUF];
};

/* */
struct res_buf_t {
    uint64_t fbuf[BUF_LEN]; // framebuffer ids
    uint64_t crtc[BUF_LEN]; // cathode ray tube (lol) controller
    uint64_t conn[BUF_LEN]; // connector e.g. vga, hdmi, etc ...
    uint64_t enco[BUF_LEN]; // encoder
};
#endif /* DRM_TYPES_H_ */
/* eof */

