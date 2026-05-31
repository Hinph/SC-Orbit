#include "constants.h"
#include "sc_gamepad_state.h"
#include "triton.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <linux/uinput.h>

int ScGamepadState_update(struct ScGamepadState* const gamepad, uint8_t const* const buffer, size_t length) {
    if (buffer[0] == TRITON_ID_IN_CONTROLLER_STATE ||
        buffer[0] == TRITON_ID_IN_CONTROLLER_STATE_BLE) {
        if (length < 28) {
            fprintf(stderr, "not enough bytes available to parse gamepad state\n");
            return RET_ERROR;
        }
        // buttons
        gamepad->btn_a          = buffer[2] & 0x01; // a
        gamepad->btn_b          = buffer[2] & 0x02; // b
        gamepad->btn_x          = buffer[2] & 0x04; // x
        gamepad->btn_y          = buffer[2] & 0x08; // y
        gamepad->btn_dpad_left  = buffer[3] & 0x10; // dpad left
        gamepad->btn_dpad_up    = buffer[3] & 0x20; // dpad up
        gamepad->btn_dpad_down  = buffer[3] & 0x04; // dpad down
        gamepad->btn_dpad_right = buffer[3] & 0x08; // dpad right
        gamepad->btn_l1         = buffer[4] & 0x08; // l1
        gamepad->btn_r1         = buffer[3] & 0x02; // r1
        gamepad->btn_l2         = buffer[5] & 0x08; // l2
        gamepad->btn_r2         = buffer[4] & 0x80; // r2
        gamepad->btn_l3         = buffer[3] & 0x80; // l3
        gamepad->btn_r3         = buffer[2] & 0x20; // r3
        gamepad->btn_l4         = buffer[4] & 0x02; // l4
        gamepad->btn_r4         = buffer[2] & 0x80; // r4
        gamepad->btn_l5         = buffer[4] & 0x04; // l5
        gamepad->btn_r5         = buffer[3] & 0x01; // r5
        gamepad->btn_base       = buffer[2] & 0x10; // quick
        gamepad->btn_start      = buffer[2] & 0x40; // menu
        gamepad->btn_select     = buffer[3] & 0x40; // view
        gamepad->btn_mode       = buffer[4] & 0x01; // steam

        // triggers
        gamepad->abs_l2 = buffer[6] | buffer[7] << 8; // l2 axis
        gamepad->abs_r2 = buffer[8] | buffer[9] << 8; // r2 axis

        // thumbsticks
        gamepad->thumbl_abs_x = buffer[10] | buffer[11] << 8; // left thumbstick x-axis
        gamepad->thumbl_abs_y = buffer[12] | buffer[13] << 8; // left thumbstick y-axis
        gamepad->thumbr_abs_x = buffer[14] | buffer[15] << 8; // right thumbstick x-axis
        gamepad->thumbr_abs_y = buffer[16] | buffer[17] << 8; // right thumbstick y-axis

        // left touchpad
        gamepad->tpl_click = buffer[5] & 0x04;             // left touchpad click
        gamepad->tpl_sense = buffer[5] & 0x02;             // left touchpad sense
        gamepad->tpl_abs_x = buffer[18] | buffer[19] << 8; // left touchpad x-axis
        gamepad->tpl_abs_y = buffer[20] | buffer[21] << 8; // left touchpad y-axis

        // right touchpad
        gamepad->tpr_click = buffer[4] & 0x40;             // right touchpad click
        gamepad->tpr_sense = buffer[4] & 0x20;             // right touchpad sense
        gamepad->tpr_abs_x = buffer[24] | buffer[25] << 8; // right touchpad x-axis
        gamepad->tpr_abs_y = buffer[26] | buffer[27] << 8; // right touchpad y-axis
    }
    return RET_OKAY;
}

void ScGamepadState_print(struct ScGamepadState const* const gamepad) {
    printf(
        "a: %d, "
        "b: %d, "
        "x: %d, "
        "y: %d\n"
        "dpad left: %d, "
        "dpad right: %d, "
        "dpad up: %d, "
        "dpad down: %d\n"
        "l1: %d, "
        "r1: %d, "
        "l2: %d, "
        "r2: %d, "
        "l3: %d, "
        "r3: %d, "
        "l4: %d, "
        "r4: %d, "
        "l5: %d, "
        "r5: %d\n"
        "left touchpad click: %d, "
        "right touchpad click: %d\n"
        "quick: %d, "
        "menu: %d, "
        "view: %d, "
        "steam: %d\n"
        "left thumbstick x-axis: %06d, "
        "left thumbstick y-axis: %06d, "
        "right thumbstick x-axis: %06d, "
        "right thumbstick y-axis: %06d\n"
        "l2-axis: %06d, "
        "r2-axis: %06d\n"
        "left touchpad x-axis: %06d, "
        "left touchpad y-axis: %06d\n"
        "right touchpad x-axis: %06d, "
        "right touchpad y-axis: %06d\n"
        "\n",
        gamepad->btn_a,
        gamepad->btn_b,
        gamepad->btn_x,
        gamepad->btn_y,
        gamepad->btn_dpad_left,
        gamepad->btn_dpad_right,
        gamepad->btn_dpad_up,
        gamepad->btn_dpad_down,
        gamepad->btn_l1,
        gamepad->btn_r1,
        gamepad->btn_l2,
        gamepad->btn_r2,
        gamepad->btn_l3,
        gamepad->btn_r3,
        gamepad->btn_l4,
        gamepad->btn_r4,
        gamepad->btn_l5,
        gamepad->btn_r5,
        gamepad->tpl_click,
        gamepad->tpr_click,
        gamepad->btn_base,
        gamepad->btn_start,
        gamepad->btn_select,
        gamepad->btn_mode,
        gamepad->thumbl_abs_x,
        gamepad->thumbl_abs_y,
        gamepad->thumbr_abs_x,
        gamepad->thumbr_abs_y,
        gamepad->abs_l2,
        gamepad->abs_r2,
        gamepad->tpl_abs_x,
        gamepad->tpl_abs_y,
        gamepad->tpr_abs_x,
        gamepad->tpr_abs_y
    );
}

static int timeval_now(struct timeval* const tv) {
    struct timespec ts;
    memset(&ts, 0, sizeof(ts));

    if (!clock_gettime(CLOCK_MONOTONIC, &ts)) {
        tv->tv_sec  = ts.tv_sec;
        tv->tv_usec = ts.tv_nsec / 1000;
        return RET_OKAY;
    }
    return RET_ERROR;
}

static int uinput_emit(
    int fd,
    uint16_t type,
    uint16_t code,
    int32_t value
) {
    struct input_event event = {
        .type  = type,
        .code  = code,
        .value = value,
    };

    if (timeval_now(&event.time)) {
        return RET_ERROR;
    }
    if (write(fd, &event, sizeof(event)) != sizeof(event)) {
        return RET_ERROR;
    }
    return RET_OKAY;
}

static int uinput_sync(int fd) {
    struct input_event event = {
        .type  = EV_SYN,
        .code  = SYN_REPORT,
        .value = 0,
    };

    if (timeval_now(&event.time)) {
        return RET_ERROR;
    }
    if (write(fd, &event, sizeof(event)) != sizeof(event)) {
        return RET_ERROR;
    }
    return RET_OKAY;
}

static int emit_key(
    int fd,
    uint16_t code,
    int32_t pval,
    int32_t cval
) {
    if (pval != cval) {
        if (uinput_emit(fd, EV_KEY, code, cval)) {
            return RET_ERROR;
        }
    }
    return RET_OKAY;
}

static int emit_abs(
    int fd,
    uint16_t code,
    int16_t pval,
    int16_t cval,
    bool negate
) {
    if (pval != cval) {
        if (uinput_emit(fd, EV_ABS, code, negate ? -cval : cval)) {
            return RET_ERROR;
        }
    }
    return RET_OKAY;
}

static int emit_keys_as_abs(
    int fd,
    uint16_t code,
    int32_t pval_pole_neg,
    int32_t pval_pole_pos,
    int32_t cval_pole_neg,
    int32_t cval_pole_pos
) {
    if ((pval_pole_neg != cval_pole_neg) ||
        (pval_pole_pos != cval_pole_pos)) {
        if (uinput_emit(fd, EV_ABS, code, cval_pole_pos - cval_pole_neg)) {
            return RET_ERROR;
        }
    }
    return RET_OKAY;
}
static int translate_buttons(
    int gamepad_fd,
    struct ScGamepadState const* const prev,
    struct ScGamepadState const* const curr
) {
    if (emit_key(gamepad_fd, BTN_SOUTH,  prev->btn_a,      curr->btn_a)      || // a
        emit_key(gamepad_fd, BTN_EAST,   prev->btn_b,      curr->btn_b)      || // b
        emit_key(gamepad_fd, BTN_NORTH,  prev->btn_x,      curr->btn_x)      || // x
        emit_key(gamepad_fd, BTN_WEST,   prev->btn_y,      curr->btn_y)      || // y
        emit_key(gamepad_fd, BTN_TL,     prev->btn_l1,     curr->btn_l1)     || // l1
        emit_key(gamepad_fd, BTN_TR,     prev->btn_r1,     curr->btn_r1)     || // r1
        emit_key(gamepad_fd, BTN_TL2,    prev->btn_l2,     curr->btn_l2)     || // l2 (full press)
        emit_key(gamepad_fd, BTN_TR2,    prev->btn_r2,     curr->btn_r2)     || // r2 (full press)
        emit_key(gamepad_fd, BTN_THUMBL, prev->btn_l3,     curr->btn_l3)     || // l3
        emit_key(gamepad_fd, BTN_THUMBR, prev->btn_r3,     curr->btn_r3)     || // r3
        emit_key(gamepad_fd, BTN_GRIPL,  prev->btn_l4,     curr->btn_l4)     || // l4
        emit_key(gamepad_fd, BTN_GRIPR,  prev->btn_r4,     curr->btn_r4)     || // r4
        emit_key(gamepad_fd, BTN_GRIPL2, prev->btn_l5,     curr->btn_l5)     || // l5
        emit_key(gamepad_fd, BTN_GRIPR2, prev->btn_r5,     curr->btn_r5)     || // r5
        emit_key(gamepad_fd, BTN_BASE,   prev->btn_base,   curr->btn_base)   || // quick
        emit_key(gamepad_fd, BTN_START,  prev->btn_start,  curr->btn_start)  || // menu
        emit_key(gamepad_fd, BTN_SELECT, prev->btn_select, curr->btn_select) || // view
        emit_key(gamepad_fd, BTN_MODE,   prev->btn_mode,   curr->btn_mode)) {   // steam
        return RET_ERROR;
    }
    return RET_OKAY;
}

static int translate_dpad(
    int gamepad_fd,
    struct ScGamepadState const* const prev,
    struct ScGamepadState const* const curr
) {
    if (emit_keys_as_abs(
        gamepad_fd,
        ABS_HAT0X,
        prev->btn_dpad_left,
        prev->btn_dpad_right,
        curr->btn_dpad_left,
        curr->btn_dpad_right
    )) {
        return RET_ERROR;
    }

    if (emit_keys_as_abs(
        gamepad_fd,
        ABS_HAT0Y,
        prev->btn_dpad_up,
        prev->btn_dpad_down,
        curr->btn_dpad_up,
        curr->btn_dpad_down
    )) {
        return RET_ERROR;
    }

    return RET_OKAY;
}

static int translate_axis(
    int gamepad_fd,
    struct ScGamepadState const* const prev,
    struct ScGamepadState const* const curr
) {
    if (emit_abs(gamepad_fd, ABS_Z,  prev->abs_l2, curr->abs_l2, false) || // l2
        emit_abs(gamepad_fd, ABS_RZ, prev->abs_r2, curr->abs_r2, false)) { // r2
        return RET_ERROR;
    }
    if (emit_abs(gamepad_fd, ABS_X,  prev->thumbl_abs_x, curr->thumbl_abs_x, false) || // left thumbstick x-axis
        emit_abs(gamepad_fd, ABS_Y,  prev->thumbl_abs_y, curr->thumbl_abs_y, true)  || // left thumbstick y-axis (negated)
        emit_abs(gamepad_fd, ABS_RX, prev->thumbr_abs_x, curr->thumbr_abs_x, false) || // right thumbstick x-axis
        emit_abs(gamepad_fd, ABS_RY, prev->thumbr_abs_y, curr->thumbr_abs_y, true)) {  // right thumbstick y-axis (negated)
        return RET_ERROR;
    }
    return RET_OKAY;
}

#define MOUSE_CLICK_HAPTICS_FREQUENCY 200 // Hz
#define MOUSE_CLICK_HAPTICS_DURATION 5 // milliseconds
#define TOUCHPAD_HAPTICS_FREQUENCY 400 // Hz
#define TOUCHPAD_HAPTICS_DURATION 5 // milliseconds
#define TOUCHPAD_HAPTICS_GRID_X 8192
#define TOUCHPAD_HAPTICS_GRID_Y 8192

static int translate_touchpad(
    int hidraw_fd,
    int mouse_fd,
    struct ScGamepadState const* const prev,
    struct ScGamepadState const* const curr
) {
    bool has_tone_l = false;
    bool has_tone_r = false;

    // should play left-click tone?
    if (!prev->tpr_click && curr->tpr_click) {
        has_tone_r = true;
        struct ScHapticsLfoTone tone = {
            .channel       = TRITON_HAPTICS_LFO_TONE_LRA_R,
            .gain          = 0,
            .frequency     = MOUSE_CLICK_HAPTICS_FREQUENCY,
            .duration_ms   = MOUSE_CLICK_HAPTICS_DURATION,
            .lfo_frequency = 0,
            .lfo_depth     = 0,
        };
        if (triton_haptics_lfo_tone(hidraw_fd, tone)) {
            return RET_ERROR;
        }
    }

    // should play right-click tone?
    if (!prev->tpl_click && curr->tpl_click) {
        has_tone_l = true;
        struct ScHapticsLfoTone tone = {
            .channel       = TRITON_HAPTICS_LFO_TONE_LRA_L,
            .gain          = 0,
            .frequency     = MOUSE_CLICK_HAPTICS_FREQUENCY,
            .duration_ms   = MOUSE_CLICK_HAPTICS_DURATION,
            .lfo_frequency = 0,
            .lfo_depth     = 0,
        };
        if (triton_haptics_lfo_tone(hidraw_fd, tone)) {
            return RET_ERROR;
        }
    }

    if (prev->tpl_sense && curr->tpl_sense) {
        const int32_t haptics_grid_x =
            curr->tpl_abs_x / TOUCHPAD_HAPTICS_GRID_X !=
            prev->tpl_abs_x / TOUCHPAD_HAPTICS_GRID_X;
        const int32_t haptics_grid_y =
            curr->tpl_abs_y / TOUCHPAD_HAPTICS_GRID_Y !=
            prev->tpl_abs_y / TOUCHPAD_HAPTICS_GRID_Y;

        if (!has_tone_l && (haptics_grid_x || haptics_grid_y)) {
            if (triton_haptics_lfo_tone(hidraw_fd, (struct ScHapticsLfoTone) {
                .channel       = TRITON_HAPTICS_LFO_TONE_LRA_L,
                .gain          = 0,
                .frequency     = TOUCHPAD_HAPTICS_FREQUENCY,
                .duration_ms   = TOUCHPAD_HAPTICS_DURATION,
                .lfo_frequency = 0,
                .lfo_depth     = 0,
            })) {
                return RET_ERROR;
            }
            has_tone_l = true;
        }
    }

    if (prev->tpr_sense && curr->tpr_sense) {
        const int32_t rel_x = (curr->tpr_abs_x - prev->tpr_abs_x) / 128;
        const int32_t rel_y = (prev->tpr_abs_y - curr->tpr_abs_y) / 128;
        if (uinput_emit(mouse_fd, EV_REL, REL_X, rel_x) ||
            uinput_emit(mouse_fd, EV_REL, REL_Y, rel_y)) {
            return RET_ERROR;
        }

        const int32_t haptics_grid_x =
            curr->tpr_abs_x / TOUCHPAD_HAPTICS_GRID_X !=
            prev->tpr_abs_x / TOUCHPAD_HAPTICS_GRID_X;
        const int32_t haptics_grid_y =
            curr->tpr_abs_y / TOUCHPAD_HAPTICS_GRID_Y !=
            prev->tpr_abs_y / TOUCHPAD_HAPTICS_GRID_Y;

        if (!has_tone_r && (haptics_grid_x || haptics_grid_y)) {
            has_tone_r = true;
            struct ScHapticsLfoTone tone = {
                .channel       = TRITON_HAPTICS_LFO_TONE_LRA_R,
                .gain          = 0,
                .frequency     = TOUCHPAD_HAPTICS_FREQUENCY,
                .duration_ms   = TOUCHPAD_HAPTICS_DURATION,
                .lfo_frequency = 0,
                .lfo_depth     = 0,
            };
            if (triton_haptics_lfo_tone(hidraw_fd, tone)) {
                return RET_ERROR;
            }
        }
    }

    if (uinput_emit(mouse_fd, EV_KEY, BTN_LEFT,  curr->tpr_click) ||
        uinput_emit(mouse_fd, EV_KEY, BTN_RIGHT, curr->tpl_click)) {
        return RET_ERROR;
    }
    return RET_OKAY;
}

int ScGamepadState_send(
    int hidraw_fd,
    int gamepad_fd,
    int mouse_fd,
    struct ScGamepadState const* const prev,
    struct ScGamepadState const* const curr
) {
    if (translate_buttons(gamepad_fd, prev, curr) ||
        translate_dpad(gamepad_fd, prev, curr) ||
        translate_axis(gamepad_fd, prev, curr) ||
        translate_touchpad(hidraw_fd, mouse_fd, prev, curr)) {
        return RET_ERROR;
    }
    if (uinput_sync(gamepad_fd) ||
        uinput_sync(mouse_fd)) {
        return RET_ERROR;
    }
    return RET_OKAY;
}
