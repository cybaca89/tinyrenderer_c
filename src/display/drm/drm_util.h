/** @file util.h
 *  @author Cy Baca
 *
 *  @brief declaration of util functions for framebuffer writer.
 */

#ifndef DRM_UTIL_H_
#define DRM_UTIL_H_
/* MEM -----------------------------------------------------------------------*/
/**
 *  @brief Get framebuffer.
 *
 *  @param fd:     const int file_descriptor of what to map.
 *  @param bytes:  const size_t number of bytes to map.
 *  @param offset: const off_t number of bytes offset from starting address.
 *  @return        pointer to first byte of mapped memory.
 */
extern void *
drm_map(/* const fd_t fd, const size_t bytes, const off_t offset */);

/* I/O -----------------------------------------------------------------------*/
/**
 */
extern void _drm_print_version_info();

/** @brief Print framebuffur var screen info to stdout.
 *
 *  @param v: const struct fb_var_screeninfo *const from <linux/fb.h> holding
 *            info about screen resolution, colors, margins, timings, etc ...
 */
extern void print_fb_var_screeninfo(
/* const struct fb_var_screeninfo *const v */);

/** @brief Print framebuffur fix screen info to stdout.
 *
 *  @param f: const struct fb_fix_screeninfo *const from <linux/fb.h> holding
 *            info about screen framebuffer memory locations, sizes, steps, etc
 */
extern void print_fb_fix_screeninfo(
/* const struct fb_fix_screeninfo *const f */);

/** @brief Print struct drm_mode_card_res info to stdout.
 *
 *  @param card_res: const struct drm_mode_card_res *const from
 *                   <libdrm/drm_mode.h>.
 */
extern void print_cardres_counts(
/* const struct drm_mode_card_res *const card_res */);

/** @brief Print modes, connectors, and framebuffer info to stdout.
 *
 *  @param get_conn: const struct drm_mode_get_connector *const from
 *                   <libdrm/drm_mode.h>. Holds mode, prop, and encoding info.
 *
 *  @param modebuf:  const struct conn_modebuf_t *const from "types.h".
 *  @param fb_base:  uint64_t fb_base, pointer address to framebuffer.(i think)
 */
extern void print_conn_and_mode_info(
/*  const struct drm_mode_get_connector *const get_conn,
    const struct conn_modebuf_t         *const modebuf,
    const uint64_t fb_base */);

/** @brief Print message to stderr.
 *
 *  TODO: add printf style params
 */
extern void drm_err(const char *msg);

/** @brief Print message to stdout.
 *
 *  TODO: add printf style params
 */
extern void drm_msg(const char *msg);
#endif /* DRM_UTIL_H_ */
/* eof */

