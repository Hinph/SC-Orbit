#include "constants.h"
#include "virtual_gamepad.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>

static int uinput_setup_axis(
    int fd,
    uint16_t code,
    uint32_t minimum,
    uint32_t maximum
) {
    struct uinput_abs_setup abs_setup = {
        .code               = code,
        .absinfo.minimum    = minimum,
        .absinfo.maximum    = maximum,
        .absinfo.flat       = 0,
        .absinfo.fuzz       = 0,
        .absinfo.resolution = 0,
    };

    if (ioctl(fd, UI_ABS_SETUP, &abs_setup)) {
        return RET_ERROR;
    }
    return RET_OKAY;
}

int virtual_gamepad_setup(int* const fd) {
    if ((*fd = open("/dev/uinput", O_RDWR | O_NONBLOCK)) < 0) {
        perror("failed to open uinput device");
        return RET_ERROR;
    }
    if (ioctl(*fd, UI_SET_EVBIT, EV_KEY) ||
        ioctl(*fd, UI_SET_EVBIT, EV_ABS) ||
        ioctl(*fd, UI_SET_EVBIT, EV_FF)) {
        perror("failed to set uinput device capabilities");
        return RET_ERROR;
    }
    if (ioctl(*fd, UI_SET_KEYBIT, BTN_SOUTH)  || // a
        ioctl(*fd, UI_SET_KEYBIT, BTN_EAST)   || // b
        ioctl(*fd, UI_SET_KEYBIT, BTN_WEST)   || // x
        ioctl(*fd, UI_SET_KEYBIT, BTN_NORTH)  || // y
        ioctl(*fd, UI_SET_ABSBIT, ABS_HAT0X)  || // dpad x-axis
        ioctl(*fd, UI_SET_ABSBIT, ABS_HAT0Y)  || // dpad y-axis
        ioctl(*fd, UI_SET_KEYBIT, BTN_TL)     || // l1
        ioctl(*fd, UI_SET_KEYBIT, BTN_TR)     || // r1
        ioctl(*fd, UI_SET_KEYBIT, BTN_TL2)    || // l2
        ioctl(*fd, UI_SET_KEYBIT, BTN_TR2)    || // r2
        ioctl(*fd, UI_SET_KEYBIT, BTN_THUMBL) || // l3
        ioctl(*fd, UI_SET_KEYBIT, BTN_THUMBR) || // r3
        ioctl(*fd, UI_SET_KEYBIT, BTN_GRIPL)  || // l4
        ioctl(*fd, UI_SET_KEYBIT, BTN_GRIPR)  || // r4
        ioctl(*fd, UI_SET_KEYBIT, BTN_GRIPL2) || // l5
        ioctl(*fd, UI_SET_KEYBIT, BTN_GRIPR2) || // r5
        ioctl(*fd, UI_SET_KEYBIT, BTN_THUMB)  || // left touchpad (TODO: unused)
        ioctl(*fd, UI_SET_KEYBIT, BTN_THUMB2) || // right touchpad (TODO: unused)
        ioctl(*fd, UI_SET_KEYBIT, BTN_BASE)   || // quick
        ioctl(*fd, UI_SET_KEYBIT, BTN_START)  || // menu
        ioctl(*fd, UI_SET_KEYBIT, BTN_SELECT) || // view
        ioctl(*fd, UI_SET_KEYBIT, BTN_MODE)   || // steam
        ioctl(*fd, UI_SET_ABSBIT, ABS_X)      || // left thumbstick x-axis
        ioctl(*fd, UI_SET_ABSBIT, ABS_Y)      || // left thumbstick y-axis
        ioctl(*fd, UI_SET_ABSBIT, ABS_RX)     || // right thumbstick x-axis
        ioctl(*fd, UI_SET_ABSBIT, ABS_RY)     || // right thumbstick y-axis
        ioctl(*fd, UI_SET_ABSBIT, ABS_Z)      || // l2
        ioctl(*fd, UI_SET_ABSBIT, ABS_RZ)) {     // r2
        perror("failed to set uinput device buttons");
        return RET_ERROR;
    }

    if (uinput_setup_axis(*fd, ABS_HAT0X, HAT_MIN,        HAT_MAX)        || // dpad x-axis
        uinput_setup_axis(*fd, ABS_HAT0Y, HAT_MIN,        HAT_MAX)        || // dpad y-axis
        uinput_setup_axis(*fd, ABS_Z,     TRIGGER_MIN,    TRIGGER_MAX)    || // l2
        uinput_setup_axis(*fd, ABS_RZ,    TRIGGER_MIN,    TRIGGER_MAX)    || // r2
        uinput_setup_axis(*fd, ABS_X,     THUMBSTICK_MIN, THUMBSTICK_MAX) || // left thumbstick x-axis
        uinput_setup_axis(*fd, ABS_Y,     THUMBSTICK_MIN, THUMBSTICK_MAX) || // left thumbstick y-axis
        uinput_setup_axis(*fd, ABS_RX,    THUMBSTICK_MIN, THUMBSTICK_MAX) || // right thumbstick x-axis
        uinput_setup_axis(*fd, ABS_RY,    THUMBSTICK_MIN, THUMBSTICK_MAX) || // right thumbstick y-axis
        uinput_setup_axis(*fd, ABS_HAT1X, TOUCHPAD_MIN,   TOUCHPAD_MAX)   || // left touchpad x-axis (TODO: unused)
        uinput_setup_axis(*fd, ABS_HAT1Y, TOUCHPAD_MIN,   TOUCHPAD_MAX)   || // left touchpad y-axis (TODO: unused)
        uinput_setup_axis(*fd, ABS_HAT2X, TOUCHPAD_MIN,   TOUCHPAD_MAX)   || // right touchpad x-axis (TODO: unused)
        uinput_setup_axis(*fd, ABS_HAT2Y, TOUCHPAD_MIN,   TOUCHPAD_MAX)) {   // right touchpad y-axis (TODO: unused)
        perror("failed to configure uinput device axis");
        return RET_ERROR;
    }
    if (ioctl(*fd, UI_SET_FFBIT, FF_RUMBLE)) {
        perror("failed to configure uinput haptics");
        return RET_ERROR;
    }

    struct uinput_setup usetup = {
        .name = VIRTUAL_GAMEPAD_NAME,
        .id = {
            .bustype = BUS_VIRTUAL,
            .vendor  = VIRTUAL_GAMEPAD_VENDOR,
            .product = VIRTUAL_GAMEPAD_PRODUCT,
            .version = VIRTUAL_GAMEPAD_VERSION,
        },
        .ff_effects_max = 1,
    };

    if (ioctl(*fd, UI_DEV_SETUP, &usetup)) {
        perror("failed to setup uinput device");
        return RET_ERROR;
    }
    if (ioctl(*fd, UI_DEV_CREATE)) {
        perror("failed to create uinput device");
        return RET_ERROR;
    }
    return RET_OKAY;
}

int virtual_gamepad_destroy(int fd) {
    int result = RET_OKAY;
    if (ioctl(fd, UI_DEV_DESTROY)) {
        perror("failed to destroy uinput device");
        result = RET_ERROR;
    }
    if (close(fd)) {
        perror("failed to close uinput device");
        result = RET_ERROR;
    }
    return result;
}

int virtual_gamepad_ff_upload(int fd, struct uinput_ff_upload* const event) {
    if (ioctl(fd, UI_BEGIN_FF_UPLOAD, event)) {
        perror("failed to upload uinput force-feedback event");
        return RET_ERROR;
    }

    event->retval = 0;

    if (ioctl(fd, UI_END_FF_UPLOAD, event)) {
        perror("failed to upload uinput force-feedback event");
        return RET_ERROR;
    }
    return RET_OKAY;
}

int virtual_gamepad_ff_erase(int fd, struct uinput_ff_erase* const event) {
    if (ioctl(fd, UI_BEGIN_FF_ERASE, event)) {
        perror("failed to erase uinput force-feedback event");
        return RET_ERROR;
    }

    event->retval = 0;

    if (ioctl(fd, UI_END_FF_ERASE, event)) {
        perror("failed to erase uinput force-feedback event");
        return RET_ERROR;
    }
    return RET_OKAY;
}
