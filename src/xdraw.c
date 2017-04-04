// #include "header/tinyrenderer.h"
#include "display.h"
#include "timer.h"
#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define OBJECT_PATH "data/african_head.obj"
#define WIDTH  800
#define HEIGHT 800
#define DEF_W 800
#define DEF_H 800
#define ONE_HALF 0.5

enum Keys { H = 43, J = 44, K = 45, L = 46 };

// static int get_input(int argc, char **argv);
/*----------------------------------------------------------------------------*/

// int
// main(int argc, char **argv)
// {
//     return x_display(argc, argv);
// }

int
x_render(const int info)
{
    model_t   *model;
    frame_t   *frame;
    display_t *dis;
    counter_t  timer = null_counter;
    uint16_t   width = DEF_W, height = DEF_H;

    /** model frame */
    model = model_create_from_wavefront(OBJECT_PATH);
    if (!model) { return EXIT_FAILURE; }
    model_scale_vertices(model, width * ONE_HALF,  height * ONE_HALF);

    frame = model_to_frame(model);
    if (!frame) {
        fprintf(stderr, "Unable to allocate segment_t\n");
        return EXIT_FAILURE;
    }
    model_destroy(model);

    /** display window */
    dis = xinit_display(width, height, info);
    if (!dis) {
        fprintf(stderr, "Failed to init display\n");
        return EXIT_SUCCESS;
    }

    /** main loop */
    int key = xevent_loop(dis, &width, &height);
    xdraw_frame(dis, frame);
    model_frame_rotate(frame, 0.1);

    while(key)
    {
        if (!counter_count(&timer))
            continue;

        key = xevent_loop(dis, &width, &height);
        int draw = 0;
        switch (key) {
            case H:
                printf("H\n");
                model_frame_translate(frame, -10.0, 0.0);
                draw = 1; break;
            case J:
                printf("J\n");
                model_frame_translate(frame, 0.0, -10.0);
                draw = 1; break;
            case K:
                printf("K\n");
                model_frame_translate(frame, 0.0, 10.0);
                draw = 1; break;
            case L:
                printf("L\n");
                model_frame_translate(frame, 10.0, 0.0);
                draw = 1; break;
            default: break;
        }

        if (draw)
            xdraw_frame(dis, frame);
    }

    /** free memory */
    extern void model_frame_destroy(frame_t *f);
    model_frame_destroy(frame);

    return EXIT_SUCCESS;
}

/*
static int
get_input(int argc, char **argv)
{
    extern int   optind;
    extern char *optarg;

    int c;
    while ((c = getopt(argc, argv, "d:v:h")) != EOF) {
        switch (c) {
            case 'd': return 'd';
            case 'h': return 'h';
            case 'v': return 'v';
            default: return 0;
        }
    }
    return 0;
}
*/
/* EOF */

