#include "gamepad.h"
#include "triton.h"
#include "utils.h"
#include "virtual_gamepad.h"
#include "virtual_mouse.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

#define GAMEPAD_CONFIG_INTERVAL 5000 // milliseconds
#define GAMEPAD_HAPTICS_INTERVAL 8 // milliseconds

int Gamepad_init(struct Gamepad* const gamepad, int epoll, int hidraw) {
    gamepad->hidraw     = hidraw;
    gamepad->ui_gamepad = -1;
    gamepad->ui_mouse   = -1;

    if (Stopwatch_init(&gamepad->sw_config, GAMEPAD_CONFIG_INTERVAL)) {
        (void)Gamepad_free(gamepad, epoll);
        return 1;
    }
    if (Stopwatch_init(&gamepad->sw_haptics, GAMEPAD_HAPTICS_INTERVAL)) {
        (void)Gamepad_free(gamepad, epoll);
        return 1;
    }
    if (virtual_mouse_setup(&gamepad->ui_mouse)) {
        (void)Gamepad_free(gamepad, epoll);
        return 1;
    }
    if (virtual_gamepad_setup(&gamepad->ui_gamepad)) {
        (void)Gamepad_free(gamepad, epoll);
        return 1;
    }
    if (epoll_ctl_add(epoll, gamepad->hidraw, EPOLLIN | EPOLLRDHUP)) {
        (void)Gamepad_free(gamepad, epoll);
        return 1;
    }
    if (epoll_ctl_add(epoll, gamepad->ui_gamepad, EPOLLIN | EPOLLRDHUP)) {
        (void)Gamepad_free(gamepad, epoll);
        return 1;
    }

    // This is required for `Gamepad_configure` to configure the controller
    // immediately. Otherwise, `GAMEPAD_CONFIG_INTERVAL` must elapse before
    // the controller no longer behaves like a mouse and keyboard.
    gamepad->sw_config.triggered = true;

    return 0;
}

int Gamepad_free(struct Gamepad* const gamepad, int epoll) {
    int result = 0;
    if (gamepad->hidraw != -1) {
        if (epoll_ctl_remove(epoll, gamepad->hidraw)) {
            result = 1;
        }
        if (close(gamepad->hidraw)) {
            perror("failed to close the hidraw device");
            result = 1;
        }
    }
    if (gamepad->ui_mouse != -1) {
        if (virtual_mouse_destroy(gamepad->ui_mouse)) {
            result = 1;
        }
    }
    if (gamepad->ui_gamepad != -1) {
        if (epoll_ctl_remove(epoll, gamepad->ui_gamepad)) {
            result = 1;
        }
        if (virtual_gamepad_destroy(gamepad->ui_gamepad)) {
            result = 1;
        }
    }
    return result;
}

int Gamepad_configure(struct Gamepad* gamepad) {
    if (Stopwatch_update(&gamepad->sw_config)) {
        return 1;
    }
    if (gamepad->sw_config.triggered) {
        if (Stopwatch_reset(&gamepad->sw_config)) {
            return 1;
        }
        if (triton_disable_lizard_mode(gamepad->hidraw)) {
            return 1;
        }
        if (triton_clear_digital_mappings(gamepad->hidraw)) {
            return 1;
        }
    }
    return 0;
}

int Gamepad_update_haptics(struct Gamepad* const gamepad) {
    if (gamepad->haptics.id == SC_HAPTICS_ID_RUMBLE) {
        if (Stopwatch_update(&gamepad->sw_haptics)) {
            return 1;
        }
        if (gamepad->sw_haptics.triggered) {
            if (Stopwatch_reset(&gamepad->sw_haptics)) {
                return 1;
            }
            if (triton_haptics_rumble(gamepad->hidraw, gamepad->haptics.u.rumble)) {
                return 1;
            }
        }
    }
    return 0;
}
