#ifndef SCWRAPPER_SC_GAMEPAD_STATE_H
#define SCWRAPPER_SC_GAMEPAD_STATE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Steam Controller
struct ScGamepadState {
    bool      btn_a;           // a
    bool      btn_b;           // b
    bool      btn_x;           // x
    bool      btn_y;           // y
    bool      btn_dpad_left;   // dpad left
    bool      btn_dpad_up;     // dpad up
    bool      btn_dpad_down;   // dpad down
    bool      btn_dpad_right;  // dpad right
    bool      btn_l1;          // l1
    bool      btn_r1;          // r1
    bool      btn_l2;          // l2 (full press)
    bool      btn_r2;          // r2 (full press)
    bool      btn_l3;          // l3
    bool      btn_r3;          // r3
    bool      btn_l4;          // l4
    bool      btn_r4;          // r4
    bool      btn_l5;          // l5
    bool      btn_r5;          // r5
    bool      btn_base;        // quick
    bool      btn_start;       // menu
    bool      btn_select;      // view
    bool      btn_mode;        // steam

    int16_t   abs_l2; // l2 axis
    int16_t   abs_r2; // r2 axis

    int16_t   thumbl_abs_x; // left thumbstick x-axis
    int16_t   thumbl_abs_y; // left thumbstick y-axis
    int16_t   thumbr_abs_x; // right thumbstick x-axis
    int16_t   thumbr_abs_y; // right thumbstick y-axis

    bool      tpl_click; // left touchpad click
    bool      tpl_sense; // left touchpad sense
    int16_t   tpl_abs_x; // left touchpad x-axis
    int16_t   tpl_abs_y; // left touchpad y-axis

    bool      tpr_click; // right touchpad click
    bool      tpr_sense; // right touchpad sense
    int16_t   tpr_abs_x; // right touchpad x-axis
    int16_t   tpr_abs_y; // right touchpad y-axis
};

int ScGamepadState_update(struct ScGamepadState* const gamepad, uint8_t const* const buffer, size_t length);
void ScGamepadState_print(struct ScGamepadState const* const gamepad);
int ScGamepadState_send(
    int hidraw_fd,
    int gamepad_fd,
    int mouse_fd,
    struct ScGamepadState const* const prev,
    struct ScGamepadState const* const curr);

#endif // SCWRAPPER_SC_GAMEPAD_STATE_H
