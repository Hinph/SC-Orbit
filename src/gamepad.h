#ifndef SCWRAPPER_GAMEPAD_H
#define SCWRAPPER_GAMEPAD_H

#include "stopwatch.h"
#include "sc_gamepad_state.h"
#include "sc_haptics.h"

struct Gamepad {
    int hidraw;
    int ui_gamepad;
    int ui_mouse;
    struct ScGamepadState state;
    struct ScHaptics haptics;
    struct Stopwatch sw_config;
    struct Stopwatch sw_haptics;
};

int Gamepad_init(struct Gamepad* const gamepad, int epoll, int hidraw);
int Gamepad_free(struct Gamepad* const gamepad, int epoll);
int Gamepad_configure(struct Gamepad* const gamepad);
int Gamepad_update_haptics(struct Gamepad* const gamepad);

#endif // SCWRAPPER_GAMEPAD_H
