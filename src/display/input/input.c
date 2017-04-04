/**
 * consert:
 * <linux/input.h>
 * <libinput.h>
 * <linux/kd.h>
 * <xkbcommon.h>
 *
 * from:  linux/Documentation/input/input.txt
 *          - mentions /hid/hiddev.txt
 *          - usbkbd also mentioned
 *          - evdev generic input interface using /dev/input
 *
 *  /dev/input/
 *  ├── by-id
 *  │   ├── usb-04d9_daskeyboard-event-if01 -> ../event1
 *  │   ├── usb-04d9_daskeyboard-event-kbd -> ../event0
 *  │   ├── usb-PIXART_USB_OPTICAL_MOUSE-event-mouse -> ../event2
 *  │   └── usb-PIXART_USB_OPTICAL_MOUSE-mouse -> ../mouse0
 *  ├── by-path
 *  │   ├── pci-0000:00:1a.0-usb-0:1.1:1.0-event-kbd -> ../event0
 *  │   ├── pci-0000:00:1a.0-usb-0:1.1:1.1-event -> ../event1
 *  │   ├── pci-0000:00:1a.0-usb-0:1.2.2:1.0-event-mouse -> ../event2
 *  │   ├── pci-0000:00:1a.0-usb-0:1.2.2:1.0-mouse -> ../mouse0
 *  │   └── platform-pcspkr-event-spkr -> ../event5
 *  ├── event0
 *  ├── event1
 *  ├── event10
 *  ├── event11
 *  ├── event12
 *  ├── event2
 *  ├── event3
 *  ├── event4
 *  ├── event5
 *  ├── event6
 *  ├── event7
 *  ├── event8
 *  ├── event9
 *  ├── mice
 *  └── mouse0
 *
 *  using select() on /dev/input/eventX will get you input events in the form
 *  of:
 *
 *      struct input_event {
 *          struct timeval time;
 *          unsigned short type;
 *          unsigned short code;
 *          unsigned int   value;
 *      };
 *-----------------------------------------------------------------------------
 */

/** @file input.c
 *  @author Cy Baca
 *
 *  @brief keyboard i/o modules.
 */

#include <stdio.h>
// #include <stdlib.h>
#include <sys/select.h>
/*
 *  typedef long int fd_mask;
 *   define NFDBITS (8 * (int)sizeof(__fd_mask))
 *   FD_SETSIZE = max num of file descriptos in fd_set
 *  typedef struct {
 *      fd_mask fds_bits[FD_SETSIZE / NFDBITS];
 *  } fd_set;
 *
 *  int select(
 *      int nfds,
 *      fd_set *restrict readfds,
 *      fd_set *restrict writefds,
 *      fd_set *restrict errorfds,
 *      struct timeval *restrict timeout);
 *  int pselect(
 *      int nfds,
 *      fd_set *restrict readfds,
 *      fd_set *restrict writefds,
 *      fd_set *restrict errorfds,
 *      const struct timespec *restrict timeout,
 *      const setset_t *restrict sigmask);
 */

#include <linux/input.h>
/*
 *  struct input_event {
 *      struct timeval time;
 *      __u16 type;
 *      __u16 code;
 *      __s32 value;
 *  };
 *
 * includes:
 * <sys/time.h>, <sys/ioctl.h>, <sys/types.h>, and <linux/types.h>
 *
 * functionality:
 *  struct input_keymap_entry {
 *      __u8 flags;
 *      __u8 len;
 *      __u16 index;
 *      __u32 keycode;
 *      __u8 scancode[32];
 *  };
 *  use with ioctl(fd, EVIOCGKEYCODE, &ike);
 *
 *  also includes forced feedback structures and methods like ff_ramp_effect
 *  (the longer event is ignored, the higher the ff priority becomes.
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
// #include <assert.h>
// #include <stropts.h>

#define KEYBOARD_DEVICE "/dev/input/event0"
typedef int fd_t;

fd_t
key_init(void)
{
    fd_t keyboard = open(KEYBOARD_DEVICE, O_RDONLY);
    if (keyboard < 0) {
        // fprintf(stderr, "Failed to aquire keyboard: %i\n", keyboard);
        perror("open keyboard");
        return 0;
    }

    return keyboard;
}

static struct input_event _ie;

void *
key_info(const fd_t *const keyboard)
{
    memset(&_ie, 0, sizeof(struct input_event));
    return read(*keyboard, &_ie, sizeof(struct input_event)) ? &_ie : NULL;
}

int
key_query(const fd_t *const keyboard)
{
    struct input_event ie = { 0 };
    return read(*keyboard, &ie, sizeof(struct input_event)) ? ie.code : 0;
}
/* eof */

