/** @file util.h
 *  @author Cy Baca
 *
 *  @brief declaration of util functions for framebuffer writer.
 */

#ifndef FRAMEBUFFER_UTIL_H_
#define FRAMEBUFFER_UTIL_H_
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
fb_map(/* const fd_t fd, const size_t bytes, const off_t offset */);

/* I/O -----------------------------------------------------------------------*/
/** @brief Print usage info to stdout.
 */
// extern void usage(void);

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

/** @brief Print message to stderr.
 *
 *  TODO: add printf style params
 */
extern void fb_err(const char *msg);

/** @brief Print message to stdout.
 *
 *  TODO: add printf style params
 */
extern void fb_msg(const char *msg);
#endif /* FRAMEBUFFER_UTIL_H_ */
/* eof */

