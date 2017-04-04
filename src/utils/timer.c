/** @file frames.c
 *  @author Cy Baca
 *
 *  @brief
 */

#include <time.h>
#include "u_types.h"

/*
struct Counter_t {
    // clock_t start;
    // clock_t prev;
    // clock_t now;
    unsigned int frames;
};
*/

unsigned int
counter_count(struct Counter_t *timer)
{
    ++timer->frames;
    // timer->prev = timer->now;
    // timer->now = clock() - timer->start;

    if (timer->frames > 60) { timer->frames -= 61; return 1; }

    return 0;
}
