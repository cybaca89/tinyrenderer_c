#ifndef FB_TYPES_H_
#define FB_TYPES_H_
#include <stdint.h>


enum TRKeyCode {
      K_NULL
    , K_ESC
    , K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9, K_0
    , K_MINUS
    , K_ASS
    , K_BS
    , K_TAB
    , K_Q, K_W, K_E, K_R, K_T, K_Y, K_U, K_I, K_O, K_P // 25
    , K_LBRACE
    , K_RBRACE
    , K_RET
    , K_LCTRL
    , K_A, K_S, K_D, K_F, K_G, K_H, K_J, K_K, K_L // 38
    , K_SC // ;
    , K_AP // '
    , K_GRAVE
    , K_LSHIFT
    , K_BACKSLASH
    , K_Z, K_X, K_C, K_V, K_B, K_N, K_M
    , K_COMMA
    , K_DOT
    , K_SLASH
    , K_RSHIFT
    , K_PASTERISK
    , K_LALT
    , K_SPACE
    , K_CL // caps
    , K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10
    , K_TOTAL
};

enum TREventType {
      IN_SYN
    , IN_PRESS
    , IN_RELEASE
};

typedef int fd_t;
typedef uint8_t byte;
#endif /* FB_TYPES_H_ */
/* eof */

