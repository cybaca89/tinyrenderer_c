/** @file   transform.c
 *  @author Cy Baca
 *
 *  @brief
 */
#include "model_types.h"
#include <math.h>

void
model_frame_translate(struct Frame_t *frame, double x, double y)
{
    uint32_t i, len = frame->size;
    struct Segment_t *segs = frame->segs;
    for (i = 0; i < len; ++i) {
        segs[i].x0 = (int16_t)((int)segs[i].x0 + (int)x);
        segs[i].y0 = (int16_t)((int)segs[i].y0 + (int)y);
        segs[i].x1 = (int16_t)((int)segs[i].x1 + (int)x);
        segs[i].y1 = (int16_t)((int)segs[i].y1 + (int)y);
    }
}

void
model_frame_rotate(struct Frame_t *frame, double rad)
{
    double c = (double)cos(rad);
    double s = (double)sin(rad);
    uint32_t i, len        = frame->size;
    struct Segment_t *segs = frame->segs;
    for (i = 0; i < len; ++i) {
        segs[i].x0 = (int16_t)((double)segs[i].x0 * c
                             + (double)segs[i].x0 * -s);
        segs[i].x1 = (int16_t)((double)segs[i].x1 * c
                             + (double)segs[i].x1 * -s);
    }
}
/* EOF */

