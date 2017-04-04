#include <linux/fb.h>
#include <stdint.h>
#include <assert.h>
#include "fb_types.h"

#define COLOR_MSG "color"
#define GRAY_MSG "gray"
#define OTHER_MSG "fourcc"

#define TRUE_MSG "yes"
#define FALSE_MSG "no"

/* MEM          --------------------------------------------------------------*/
#include <sys/mman.h> // mmap() for accessing framebuffer space to write to
void *
fb_map(const fd_t fd, const size_t bytes, const off_t offset)
{
    assert(fd > 2);
    assert(bytes > 0);
    return mmap(0, bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
}

/* IO <stdio.h> --------------------------------------------------------------*/
#include <stdio.h>
/*
void
usage(void)
{
    printf(
    "usage:\n-v: screen info\n-h: this message\n-r [opt]:\n\t opt:\n"
    "\t     fb  -render directly to framebuffer\n"
    "\t     drm -render using drm driver\n\n");
}
*/

/** from <linux/fb.h> */
// struct fb_var_screeninfo {
//     __u32 xres;                /* visible resolution                    */
//     __u32 yres;
//     __u32 xres_virtual;        /* virtual resolution                    */
//     __u32 yres_virtual;
//     __u32 xoffset;             /* offset from virtual to visible        */
//     __u32 yoffset;             /* resolution                            */
//     __u32 bits_per_pixel;      /* guess what                            */
//     __u32 grayscale;           /* 0 = color, 1 = grayscale, >1 = FOURCC */
//     struct fb_bitfield red;    /* bitfield in fb mem if true color,     */
//     struct fb_bitfield green;  /* else only length is significant       */
//     struct fb_bitfield blue;
//     struct fb_bitfield transp; /* transparency                          */
//     __u32 nonstd;              /* != 0 Non standard pixel format        */
//     __u32 activate;            /* see FB_ACTIVATE_*                     */
//     __u32 height;              /* height of picture in mm               */
//     __u32 width;               /* width of picture in mm                */
//     __u32 accel_flags;         /* (OBSOLETE) see fb_info.flags          */
//     /* Timing: All values in pixclocks, except pixclock (of course)     */
//     __u32 pixclock;            /* pixel clock in ps (pico seconds)      */
//     __u32 left_margin;         /* time from sync to picture             */
//     __u32 right_margin;        /* time from picture to sync             */
//     __u32 upper_margin;        /* time from sync to picture             */
//     __u32 lower_margin;
//     __u32 hsync_len;           /* length of horizontal sync             */
//     __u32 vsync_len;           /* length of vertical sync               */
//     __u32 sync;                /* see FB_SYNC_*                         */
//     __u32 vmode;               /* see FB_VMODE_*                        */
//     __u32 rotate;              /* angle we rotate counter clockwise     */
//     __u32 colorspace;          /* colorspace for FOURCC-based modes     */
//     __u32 reserved[4];         /* Reserved for future compatibility     */
// };

void
print_fb_var_screeninfo(const struct fb_var_screeninfo *const v)
{
    assert(v);
    printf(
    "VAR SCREENINFO\n"
    "visiable resolution: %ux%u\n"
    "virtual resolution:  %ux%u\n"
    "offset:              %ux%u\n"
    "bits per pixel:      %u\n"
    "grayscale:           %s\n"
    "...\n"
    "standard px format?  %s\n"
    "FB_ACTIVATE:         %u\n"
    "height:              %umm\n"
    "width:               %umm\n"
    "time...\n"
    "\tpixclock: %u\n"
    "\tfrom sync to pic  left margin %u\n"
    "\tfrom pic to sync right margin %u\n"
    "\tfrom sync to pic upper margin %u\n"
    "\tfrom pic to sync lower margin %u\n"
    "hsync len: %u\n"
    "vsync len: %u\n"
    "FB_SYNC:   %u\n"
    "FB_VMODE:  %u\n"
    "cc rotate angle:                   %u\n"
    "colorpsace for FOURCC based modes: %u\n\n\n"
    , v->xres, v->yres
    , v->xres_virtual, v->yres_virtual
    , v->xoffset, v->yoffset
    , v->bits_per_pixel
    , v->grayscale ? v->grayscale > 1 ? OTHER_MSG : GRAY_MSG : COLOR_MSG
    , v->nonstd ? FALSE_MSG : TRUE_MSG
    , v->activate
    , v->height
    , v->width
    , v->pixclock
    , v->left_margin
    , v->right_margin
    , v->upper_margin
    , v->lower_margin
    , v->hsync_len
    , v->vsync_len
    , v->sync
    , v->vmode
    , v->rotate
    , v->colorspace
    );
}

/** from <linux/fb.h> */
// struct fb_fix_screeninfo {
//     char id[16];              /* identification string eg "TT Builtin"     */
//     unsigned long smem_start; /* Start of frame buffer mem (phys. address) */
//     __u32 smem_len;           /* Length of frame buffer mem                */
//     __u32 type;               /* see FB_TYPE_*                             */
//     __u32 type_aux;           /* Interleave for interleaved Planes         */
//     __u32 visual;             /* see FB_VISUAL_*                           */
//     __u16 xpanstep;           /* zero if no hardware panning               */
//     __u16 ypanstep;           /* zero if no hardware panning               */
//     __u16 ywrapstep;          /* zero if no hardware ywrap                 */
//     __u32 line_length;        /* length of a line in bytes                 */
//     unsigned long mmio_start; /* Start of Memory Mapped I/O(phys. address) */
//     __u32 mmio_len;           /* Length of Memory Mapped I/O               */
//     __u32 accel;              /* Tell driver which specific chip/card      */
//     __u16 capabilities;       /* see FB_CAP_*                              */
//     __u16 reserved[2];        /* Reserved for future compatibility         */
// };
void
print_fb_fix_screeninfo(struct fb_fix_screeninfo *f)
{
    assert(f);
    printf(
    "FIX SCREENINFO\n"
    "ID:              %s\n"
    "FB phys address: 0x%lx\n"
    "FB length:       %u bytes\n"
    "FB_TYPE:         %u\n"
    "FB_TYPE_AUX:     %u\n"
    "FB_VISUAL:       %u\n"
    "x panstep?       %s\n"
    "y panstep?       %s\n"
    "y wrapstep?      %s\n"
    "line length      %u bytes\n"
    "mmapp\'d io phys addr: 0x%lx\n"
    "length of mmapp\'d io: %u\n"
    "Which specific card:   %u\n"
    "FB_CAP:                %u\n\n\n"
    , f->id
    , f->smem_start
    , f->smem_len
    , f->type
    , f->type_aux
    , f->visual
    , f->xpanstep  ? TRUE_MSG : FALSE_MSG
    , f->ypanstep  ? TRUE_MSG : FALSE_MSG
    , f->ywrapstep ? TRUE_MSG : FALSE_MSG
    , f->line_length
    , f->mmio_start
    , f->mmio_len
    , f->accel
    , f->capabilities
    );
}

void
fb_err(const char *msg) {
    assert(msg);
    fprintf(stderr, msg);
}

void
fb_msg(const char *msg) {
    assert(msg);
    printf(msg);
}
/* eof */

