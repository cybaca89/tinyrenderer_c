#ifndef LOCAL_DRM_WRITE_H_
#define LOCAL_DRM_WRITE_H_
#include "drm_types.h" // has <stdint.h>
struct drm_mode_get_connector;
struct drm_mode_create_dumb;
struct drm_mode_modeinfo;
struct drm_mode_map_dumb;
struct drm_mode_card_res;
struct drm_mode_fb_cmd;

/* drm */
/**
 */
static int _drm_get_conn_resources(
const fd_t,
struct drm_mode_get_connector *get_conn,
struct conn_modebuf_t *modebuf,
uint64_t *conn);

/**
 */
static void *_drm_init_dumb_buffer(
const fd_t dri,
struct drm_mode_modeinfo *conn_mode_buf,
struct drm_mode_create_dumb *create_dumb,
struct drm_mode_map_dumb *map_dumb,
struct drm_mode_fb_cmd *cmd_dumb);

/**
 */
static void _drm_get_card_info(
const fd_t dri,
struct drm_mode_card_res *card_res,
struct res_buf_t *res_buf);
#endif /* LOCAL_DRM_WRITE_H_ */
/* eof */

