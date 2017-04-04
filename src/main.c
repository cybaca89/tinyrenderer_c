/** @file main.c
 *  @author Cy Baca
 *
 *  @brief Handle args and call funcs from fb_write.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>   // strcmp()
#include <unistd.h>   // getopt();
#include "fb_write.h"
#include "drm_write.h"

static int
usage(void)
{
    return printf(
    "usage:\n-v: screen info\n-h: this message\n"
    "-p [opt]:\n\t opt:\n"
    "\t     fb  -screen info\n"
    "\t     drm -drm driver info\n"
    "\t     drm -x11 screen info\n"
    "-r [opt]:\n\t opt:\n"
    "\t     fb  -render directly to framebuffer\n"
    "\t     drm -render using drm driver\n\n"
    "\t     x11 -render using xcb/x11\n\n"
    );
}

extern int x_render(const int info);

/* ENTRY POINT -------------------------------------------------------------- */
int
main(int argc, char **argv)
#define OPTSTR  "vhr:p:"
#define FB_ARG  "fb"
#define DRM_ARG "drm"
#define X_ARG   "x11"
{
    extern int   optind;
    extern char *optarg;
    int opt;
    while ((opt = getopt(argc, argv, OPTSTR)) != -1 ) {
        switch (opt) {
            case 'v': return fb_screen_info();
            case 'h': return usage();
            case 'p':
                if (strcmp(optarg, FB_ARG ) == 0) return fb_screen_info();
                if (strcmp(optarg, DRM_ARG) == 0) return drm_version_info();
                if (strcmp(optarg, X_ARG) == 0) return x_render(1);
            case 'r':
                if (strcmp(optarg, FB_ARG ) == 0) return fb_render();
                if (strcmp(optarg, DRM_ARG) == 0) return drm_render();
                if (strcmp(optarg, X_ARG) == 0) return x_render(0);
            default: break; // return framebuf_render();
        }
    }
    if (opt == -1) return fb_render();
    return EXIT_SUCCESS;
}
/* eof */

