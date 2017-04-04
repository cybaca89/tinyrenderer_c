#ifndef TR_DISPLAY_H_
#define TR_DISPLAY_H_

// #include "types.h"
struct Display_t;
#include <stdint.h>

/** display.c       ----------------------------------------------------------*/
/** @brief
 *
 *  @param
 *  @param
 *  @param
 *  @return
 */
extern void *xinit_display(const uint16_t w, const uint16_t h, const int opt);

/** @brief
 *
 *  @param
 *  @return
 */
extern int xevent_loop(struct Display_t *dis, uint16_t *restrict w
, uint16_t *restrict h);

// extern void xdraw_frame(struct Display_t *dis, void *segs, const uint32_t SIZE);
extern void xdraw_frame(struct Display_t *dis, void *frame);

typedef struct Display_t display_t;

#endif /* TR_DISPLAY_H_ */
/* EOF */

