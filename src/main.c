#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <linux/hidraw.h>
#include <linux/uinput.h>

int safe_strcpy(char* const dst, char const* const src, size_t count) {
    if ((count == 0) || (count <= strlen(src))) {
        return 1;
    }

    strcpy(dst, src);

    return 0;
}

#ifndef BTN_GRIPL
#define BTN_GRIPL BTN_0
#endif

#ifndef BTN_GRIPR
#define BTN_GRIPR BTN_1
#endif

#ifndef BTN_GRIPL2
#define BTN_GRIPL2 BTN_2
#endif

#ifndef BTN_GRIPR2
#define BTN_GRIPR2 BTN_3
#endif

// steam controller
struct sc_gamepad_state {
    bool    btn_south;        // a
    bool    btn_east;         // b
    bool    btn_west;         // x
    bool    btn_north;        // y
    bool    btn_dpad_left;    // dpad left
    bool    btn_dpad_up;      // dpad up
    bool    btn_dpad_down;    // dpad down
    bool    btn_dpad_right;   // dpad right
    bool    btn_tl;           // l1
    bool    btn_tr;           // r1
    bool    btn_tl2;          // l2 (full press)
    bool    btn_tr2;          // r2 (full press)
    bool    btn_thumbl;       // l3
    bool    btn_thumbr;       // r3
    bool    btn_gripl;        // l4
    bool    btn_gripr;        // r4
    bool    btn_gripl2;       // l5
    bool    btn_gripr2;       // r5
    bool    btn_thumb;        // touchpad-l click
    bool    btn_thumb2;       // touchpad-r click
    bool    btn_base;         // quick
    bool    btn_start;        // menu
    bool    btn_select;       // view
    bool    btn_mode;         // steam
    int16_t   abs_x;          // thumbstick-l x-axis
    int16_t   abs_y;          // thumbstick-l y-axis
    int16_t   abs_rx;         // thumbstick-r x-axis
    int16_t   abs_ry;         // thumbstick-r y-axis
    int16_t   abs_z;          // l2
    int16_t   abs_rz;         // r2
    int16_t   abs_hat1x;      // touchpad-l x-axis
    int16_t   abs_hat1y;      // touchpad-l y-axis
    int16_t   abs_hat2x;      // touchpad-r x-axis
    int16_t   abs_hat2y;      // touchpad-r y-axis
    int8_t    mouse_l;        // mouse l click (unmapped)
    int8_t    mouse_r;        // mouse r click (unmapped)
    int8_t    mouse_rel_x;    // mouse relative x (unmapped)
    int8_t    mouse_rel_y;    // mouse relative y (unmapped)
    int8_t    mouse_wheel_x;  // mouse wheel x (unmapped)
    int8_t    mouse_wheel_y;  // mouse wheel y (unmapped)
};

void sc_gamepad_state_update(struct sc_gamepad_state* const gamepad, uint8_t const* const buffer) {
    // Report ID (64)
    if (buffer[0] == 64) {
        gamepad->mouse_l       = buffer[1] & 0x1; // mouse l click
        gamepad->mouse_r       = buffer[1] & 0x2; // mouse r click
        gamepad->mouse_rel_x   = buffer[2];       // mouse relative x
        gamepad->mouse_rel_y   = buffer[3];       // mouse relative y
        gamepad->mouse_wheel_y = buffer[4];       // mouse wheel x
        gamepad->mouse_wheel_x = buffer[5];       // mouse wheel y
    }
    // Report ID (66)
    if (buffer[0] == 66) {
        gamepad->btn_south      = buffer[2] & 0x01;             // a
        gamepad->btn_east       = buffer[2] & 0x02;             // b
        gamepad->btn_west       = buffer[2] & 0x04;             // x
        gamepad->btn_north      = buffer[2] & 0x08;             // y
        gamepad->btn_dpad_left  = buffer[3] & 0x10;             // dpad left
        gamepad->btn_dpad_up    = buffer[3] & 0x20;             // dpad up
        gamepad->btn_dpad_down  = buffer[3] & 0x04;             // dpad down
        gamepad->btn_dpad_right = buffer[3] & 0x08;             // dpad right
        gamepad->btn_tl         = buffer[4] & 0x08;             // l1
        gamepad->btn_tr         = buffer[3] & 0x02;             // r1
        gamepad->btn_tl2        = buffer[5] & 0x08;             // l2
        gamepad->btn_tr2        = buffer[4] & 0x80;             // r2
        gamepad->btn_thumbl     = buffer[3] & 0x80;             // l3
        gamepad->btn_thumbr     = buffer[2] & 0x20;             // r3
        gamepad->btn_gripl      = buffer[4] & 0x02;             // l4
        gamepad->btn_gripr      = buffer[2] & 0x80;             // r4
        gamepad->btn_gripl2     = buffer[4] & 0x04;             // l5
        gamepad->btn_gripr2     = buffer[3] & 0x01;             // r5
        gamepad->btn_thumb      = buffer[5] & 0x04;             // touchpad-l click
        gamepad->btn_thumb2     = buffer[4] & 0x40;             // touchpad-r click
        gamepad->btn_base       = buffer[2] & 0x10;             // quick
        gamepad->btn_start      = buffer[2] & 0x40;             // menu
        gamepad->btn_select     = buffer[3] & 0x40;             // view
        gamepad->btn_mode       = buffer[4] & 0x01;             // steam
        gamepad->abs_x          = buffer[10] | buffer[11] << 8; // thumbstick-l x-axis
        gamepad->abs_y          = buffer[12] | buffer[13] << 8; // thumbstick-l y-axis
        gamepad->abs_rx         = buffer[14] | buffer[15] << 8; // thumbstick-r x-axis
        gamepad->abs_ry         = buffer[16] | buffer[17] << 8; // thumbstick-r y-axis
        gamepad->abs_z          = buffer[ 6] | buffer[ 7] << 8; // l2
        gamepad->abs_rz         = buffer[ 8] | buffer[ 9] << 8; // r2
        gamepad->abs_hat1x      = buffer[18] | buffer[19] << 8; // touchpad-l x-axis
        gamepad->abs_hat1y      = buffer[20] | buffer[21] << 8; // touchpad-l y-axis
        gamepad->abs_hat2x      = buffer[24] | buffer[25] << 8; // touchpad-r x-axis
        gamepad->abs_hat2y      = buffer[26] | buffer[27] << 8; // touchpad-r y-axis
    }
}

void sc_gamepad_state_print(struct sc_gamepad_state const* const gamepad) {
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
        "touchpad-l press: %d, "
        "touchpad-r press: %d\n"
        "quick: %d, "
        "menu: %d, "
        "view: %d, "
        "steam: %d\n"
        "thumbstick-l x: %06d, "
        "thumbstick-l y: %06d, "
        "thumbstick-r x: %06d, "
        "thumbstick-r y: %06d\n"
        "l2: %06d, "
        "r2: %06d\n"
        "touchpad-l x: %06d, "
        "touchpad-l y: %06d\n"
        "touchpad-r x: %06d, "
        "touchpad-r y: %06d\n"
        "mouse l: %d, "
        "mouse r: %d, "
        "mouse rel-x: %03d, "
        "mouse rel-y: %03d, "
        "mouse wheel-x: %03d, "
        "mouse wheel-y: %03d\n"
        "\n",
        gamepad->btn_south,
        gamepad->btn_east,
        gamepad->btn_west,
        gamepad->btn_north,
        gamepad->btn_dpad_left,
        gamepad->btn_dpad_right,
        gamepad->btn_dpad_up,
        gamepad->btn_dpad_down,
        gamepad->btn_tl,
        gamepad->btn_tr,
        gamepad->btn_tl2,
        gamepad->btn_tr2,
        gamepad->btn_thumbl,
        gamepad->btn_thumbr,
        gamepad->btn_gripl,
        gamepad->btn_gripr,
        gamepad->btn_gripl2,
        gamepad->btn_gripr2,
        gamepad->btn_thumb,
        gamepad->btn_thumb2,
        gamepad->btn_base,
        gamepad->btn_start,
        gamepad->btn_select,
        gamepad->btn_mode,
        gamepad->abs_x,
        gamepad->abs_y,
        gamepad->abs_rx,
        gamepad->abs_ry,
        gamepad->abs_z,
        gamepad->abs_rz,
        gamepad->abs_hat1x,
        gamepad->abs_hat1y,
        gamepad->abs_hat2x,
        gamepad->abs_hat2y,
        gamepad->mouse_l,
        gamepad->mouse_r,
        gamepad->mouse_rel_x,
        gamepad->mouse_rel_y,
        gamepad->mouse_wheel_x,
        gamepad->mouse_wheel_y
    );
}

static int timeval_now(struct timeval* const tv) {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        tv->tv_sec  = ts.tv_sec;
        tv->tv_usec = ts.tv_nsec / 1000;
        return 0;
    }
    return 1;
}

static int uinput_send(
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
        return 1;
    }
    if (write(fd, &event, sizeof(event)) != sizeof(event)) {
        return 1;
    }
    return 0;
}

static int uinput_sync(int fd) {
    struct input_event event = {
        .type  = EV_SYN,
        .code  = SYN_REPORT,
        .value = 0,
    };
    if (timeval_now(&event.time)) {
        return 1;
    }
    if (write(fd, &event, sizeof(event)) != sizeof(event)) {
        return 1;
    }
    return 0;
}

static int send_key(
    int fd,
    uint16_t code,
    int32_t pval,
    int32_t cval
) {
    if (pval != cval) {
        if (uinput_send(fd, EV_KEY, code, cval)) {
            return 1;
        }
    }
    return 0;
}

static int send_abs(
    int fd,
    uint16_t code,
    int16_t pval,
    int16_t cval,
    bool negate
) {
    if (pval != cval) {
        if (uinput_send(fd, EV_ABS, code, negate ? -cval : cval)) {
            return 1;
        }
    }
    return 0;
}

static int send_keys_as_abs(
    int fd,
    uint16_t code,
    int32_t pval_pole_neg,
    int32_t pval_pole_pos,
    int32_t cval_pole_neg,
    int32_t cval_pole_pos
) {
    if ((pval_pole_neg != cval_pole_neg) ||
        (pval_pole_pos != cval_pole_pos)) {
        if (uinput_send(fd, EV_ABS, code, cval_pole_pos - cval_pole_neg)) {
            return 1;
        }
    }
    return 0;
}

int sc_gamepad_state_send(
    int uinput_fd,
    struct sc_gamepad_state const* const prev,
    struct sc_gamepad_state const* const curr
) {
    send_key(uinput_fd, BTN_SOUTH,  prev->btn_south,  curr->btn_south);  // a
    send_key(uinput_fd, BTN_EAST,   prev->btn_east,   curr->btn_east);   // b
    send_key(uinput_fd, BTN_NORTH,  prev->btn_west,   curr->btn_west);   // x (swapped with Y)
    send_key(uinput_fd, BTN_WEST,   prev->btn_north,  curr->btn_north);  // y (swapped with X)
    send_key(uinput_fd, BTN_TL,     prev->btn_tl,     curr->btn_tl);     // l1
    send_key(uinput_fd, BTN_TR,     prev->btn_tr,     curr->btn_tr);     // r1
    send_key(uinput_fd, BTN_TL2,    prev->btn_tl2,    curr->btn_tl2);    // l2 (full press)
    send_key(uinput_fd, BTN_TR2,    prev->btn_tr2,    curr->btn_tr2);    // r2 (full press)
    send_key(uinput_fd, BTN_THUMBL, prev->btn_thumbl, curr->btn_thumbl); // l3
    send_key(uinput_fd, BTN_THUMBR, prev->btn_thumbr, curr->btn_thumbr); // r3
    send_key(uinput_fd, BTN_GRIPL,  prev->btn_gripl,  curr->btn_gripl);  // l4
    send_key(uinput_fd, BTN_GRIPR,  prev->btn_gripr,  curr->btn_gripr);  // r4
    send_key(uinput_fd, BTN_GRIPL2, prev->btn_gripl2, curr->btn_gripl2); // l5
    send_key(uinput_fd, BTN_GRIPR2, prev->btn_gripr2, curr->btn_gripr2); // r5
    send_key(uinput_fd, BTN_THUMB,  prev->btn_thumb,  curr->btn_thumb);  // touchpad l-click
    send_key(uinput_fd, BTN_THUMB2, prev->btn_thumb2, curr->btn_thumb2); // touchpad r-click
    send_key(uinput_fd, BTN_BASE,   prev->btn_base,   curr->btn_base);   // quick
    send_key(uinput_fd, BTN_START,  prev->btn_start,  curr->btn_start);  // menu
    send_key(uinput_fd, BTN_SELECT, prev->btn_select, curr->btn_select); // view
    send_key(uinput_fd, BTN_MODE,   prev->btn_mode,   curr->btn_mode);   // steam
    send_abs(uinput_fd, ABS_Z,      prev->abs_z,      curr->abs_z,     false); // l2
    send_abs(uinput_fd, ABS_RZ,     prev->abs_rz,     curr->abs_rz,    false); // r2
    send_abs(uinput_fd, ABS_X,      prev->abs_x,      curr->abs_x,     false); // thumbstick-l x-axis
    send_abs(uinput_fd, ABS_Y,      prev->abs_y,      curr->abs_y,     true);  // thumbstick-l y-axis (negated)
    send_abs(uinput_fd, ABS_RX,     prev->abs_rx,     curr->abs_rx,    false); // thumbstick-r x-axis
    send_abs(uinput_fd, ABS_RY,     prev->abs_ry,     curr->abs_ry,    true);  // thumbstick-r y-axis (negated)
    send_abs(uinput_fd, ABS_HAT1X,  prev->abs_hat1x,  curr->abs_hat1x, false); // touchpad-l x-axis
    send_abs(uinput_fd, ABS_HAT1Y,  prev->abs_hat1y,  curr->abs_hat1y, true);  // touchpad-l y-axis (negated)
    send_abs(uinput_fd, ABS_HAT2X,  prev->abs_hat2x,  curr->abs_hat2x, false); // touchpad-r x-axis
    send_abs(uinput_fd, ABS_HAT2Y,  prev->abs_hat2y,  curr->abs_hat2y, true);  // touchpad-r y-axis (negated)
    send_keys_as_abs(
        uinput_fd,
        ABS_HAT0X,
        prev->btn_dpad_left,
        prev->btn_dpad_right,
        curr->btn_dpad_left,
        curr->btn_dpad_right
    );
    send_keys_as_abs(
        uinput_fd,
        ABS_HAT0Y,
        prev->btn_dpad_up,
        prev->btn_dpad_down,
        curr->btn_dpad_up,
        curr->btn_dpad_down
    );
    uinput_sync(uinput_fd);
    return 0;
}

#define GAMEPAD_MAX_SLOTS 16
struct gamepad_slots {
    struct gamepad_slot {
        char device[NAME_MAX];
        int hidraw_fd;
        int uinput_fd;
        struct sc_gamepad_state gamepad;
    } slots[GAMEPAD_MAX_SLOTS];
    size_t count;
};

int gamepad_slot_insert(
    struct gamepad_slots* const slots,
    char const* const device,
    int hidraw_fd,
    int uinput_fd
) {
    for (size_t i = 0; i < slots->count; i++) {
        if (!strcmp(slots->slots[i].device, device)) {
            fprintf(stderr, "gamepad device already monitored\n");
            return 1;
        }
    }

    if (slots->count < GAMEPAD_MAX_SLOTS) {
        struct gamepad_slot* const slot = &slots->slots[slots->count];
        if (safe_strcpy(slot->device, device, NAME_MAX)) {
            fprintf(stderr, "gamepad device name too long\n");
            return 1;
        }
        slot->hidraw_fd = hidraw_fd;
        slot->uinput_fd = uinput_fd;
        memset(&slot->gamepad, 0, sizeof(slot->gamepad));
        slots->count += 1;
        return 0;
    }

    fprintf(stderr, "failed to monitor gamepad device\n");
    return 1;
}

int gamepad_slot_remove(struct gamepad_slots* const slots, size_t index) {
    if (index >= slots->count) {
        fprintf(stderr, "failed to remove non-existent gamepad slot\n");
        return 1;
    }

    struct gamepad_slot* const dst = &slots->slots[index];
    struct gamepad_slot* const src = &slots->slots[slots->count - 1];

    memmove(dst->device, src->device, NAME_MAX);
    dst->hidraw_fd = src->hidraw_fd;
    dst->uinput_fd = src->uinput_fd;

    slots->count -= 1;

    return 0;
}

#define HAT_MIN        -1
#define HAT_MAX         1
#define TRIGGER_MIN     0
#define TRIGGER_MAX     32767
#define THUMBSTICK_MIN -32767
#define THUMBSTICK_MAX  32767
#define TOUCHPAD_MIN   -32767
#define TOUCHPAD_MAX    32767

static int setup_axis(int fd, uint16_t code, uint32_t min, uint32_t max) {
    struct uinput_abs_setup abs_setup = {
        .code               = code,
        .absinfo.minimum    = min,
        .absinfo.maximum    = max,
        .absinfo.flat       = 0,
        .absinfo.fuzz       = 0,
        .absinfo.resolution = 0,
    };

    if (ioctl(fd, UI_ABS_SETUP, &abs_setup)) {
        return 1;
    }

    return 0;
}

int virtual_gamepad_setup(int* const uinput_fd) {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("failed to open uinput device");
        return 1;
    }

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) ||
        ioctl(fd, UI_SET_EVBIT, EV_ABS)) {
        perror("failed to set uinput device capabilities");
        return 1;
    }

    if (ioctl(fd, UI_SET_KEYBIT, BTN_SOUTH)  || // a
        ioctl(fd, UI_SET_KEYBIT, BTN_EAST)   || // b
        ioctl(fd, UI_SET_KEYBIT, BTN_WEST)   || // x
        ioctl(fd, UI_SET_KEYBIT, BTN_NORTH)  || // y
        ioctl(fd, UI_SET_ABSBIT, ABS_HAT0X)  || // dpad x-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_HAT0Y)  || // dpad y-axis
        ioctl(fd, UI_SET_KEYBIT, BTN_TL)     || // l1
        ioctl(fd, UI_SET_KEYBIT, BTN_TR)     || // r1
        ioctl(fd, UI_SET_KEYBIT, BTN_TL2)    || // l2
        ioctl(fd, UI_SET_KEYBIT, BTN_TR2)    || // r2
        ioctl(fd, UI_SET_KEYBIT, BTN_THUMBL) || // l3
        ioctl(fd, UI_SET_KEYBIT, BTN_THUMBR) || // r3
        ioctl(fd, UI_SET_KEYBIT, BTN_GRIPL)  || // l4
        ioctl(fd, UI_SET_KEYBIT, BTN_GRIPR)  || // r4
        ioctl(fd, UI_SET_KEYBIT, BTN_GRIPL2) || // l5
        ioctl(fd, UI_SET_KEYBIT, BTN_GRIPR2) || // r5
        ioctl(fd, UI_SET_KEYBIT, BTN_THUMB)  || // touchpad-l click
        ioctl(fd, UI_SET_KEYBIT, BTN_THUMB2) || // touchpad-r click
        ioctl(fd, UI_SET_KEYBIT, BTN_BASE)   || // quick
        ioctl(fd, UI_SET_KEYBIT, BTN_START)  || // menu
        ioctl(fd, UI_SET_KEYBIT, BTN_SELECT) || // view
        ioctl(fd, UI_SET_KEYBIT, BTN_MODE)   || // steam
        ioctl(fd, UI_SET_ABSBIT, ABS_X)      || // l-thumbstick x-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_Y)      || // l-thumbstick y-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_RX)     || // r-thumbstick x-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_RY)     || // r-thumbstick y-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_Z)      || // l2
        ioctl(fd, UI_SET_ABSBIT, ABS_RZ)     || // r2
        ioctl(fd, UI_SET_ABSBIT, ABS_HAT1X)  || // touchpad-l x-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_HAT1Y)  || // touchpad-l y-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_HAT2X)  || // touchpad-r x-axis
        ioctl(fd, UI_SET_ABSBIT, ABS_HAT2Y)) {  // touchpad-r y-axis
        perror("failed to configure uinput device buttons");
        return 1;
    }

    if (setup_axis(fd, ABS_HAT0X, HAT_MIN,        HAT_MAX)        || // dpad x-axis
        setup_axis(fd, ABS_HAT0Y, HAT_MIN,        HAT_MAX)        || // dpad y-axis
        setup_axis(fd, ABS_Z,     TRIGGER_MIN,    TRIGGER_MAX)    || // l2
        setup_axis(fd, ABS_RZ,    TRIGGER_MIN,    TRIGGER_MAX)    || // r2
        setup_axis(fd, ABS_X,     THUMBSTICK_MIN, THUMBSTICK_MAX) || // l-thumbstick x-axis
        setup_axis(fd, ABS_Y,     THUMBSTICK_MIN, THUMBSTICK_MAX) || // l-thumbstick y-axis
        setup_axis(fd, ABS_RX,    THUMBSTICK_MIN, THUMBSTICK_MAX) || // r-thumbstick x-axis
        setup_axis(fd, ABS_RY,    THUMBSTICK_MIN, THUMBSTICK_MAX) || // r-thumbstick y-axis
        setup_axis(fd, ABS_HAT1X, TOUCHPAD_MIN,   TOUCHPAD_MAX)   || // touchpad-l x-axis
        setup_axis(fd, ABS_HAT1Y, TOUCHPAD_MIN,   TOUCHPAD_MAX)   || // touchpad-l y-axis
        setup_axis(fd, ABS_HAT2X, TOUCHPAD_MIN,   TOUCHPAD_MAX)   || // touchpad-r x-axis
        setup_axis(fd, ABS_HAT2Y, TOUCHPAD_MIN,   TOUCHPAD_MAX)) {   // touchpad-r y-axis
        perror("failed to configure uinput device axis");
        return 1;
    }

    struct uinput_setup usetup = {
        .name = "Virtual Controller",
        .id = {
            .bustype = BUS_VIRTUAL,
            .vendor  = 0x0000,
            .product = 0x0000,
            .version = 1,
        }
    };

    if (ioctl(fd, UI_DEV_SETUP, &usetup)) {
        perror("failed to setup uinput device");
        return 1;
    }

    if (ioctl(fd, UI_DEV_CREATE)) {
        perror("failed to create uinput device");
        return 1;
    }

    *uinput_fd = fd;

    return 0;
}

int virtual_gamepad_destroy(int fd) {
    if (ioctl(fd, UI_DEV_DESTROY)) {
        perror("failed to destroy virtual gamepad");
        return 1;
    }

    if (close(fd)) {
        perror("failed to close virtual gamepad");
        return 1;
    }

    return 0;
}

static void cleanup_fd(int *fd) {
    if (close(*fd)) {
        perror("failed to close file-descriptor");
    }
}

static void cleanup_dir(DIR** dir) {
    if (closedir(*dir)) {
        perror("failed to close directory");
    }
}

static int epoll_ctl_add(int epoll_fd, int fd, uint32_t events) {
    struct epoll_event event = {
        .data.fd = fd,
        .events  = events,
    };
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event)) {
        perror("failed to add file-descriptor to epoll");
        return 1;
    }
    return 0;
}

static int epoll_ctl_remove(int epoll_fd, int fd) {
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL)) {
        perror("failed to remove file-descriptor from epoll");
        return 1;
    }
    return 0;
}

int maybe_add_gamepad(
    struct gamepad_slots* const slots,
    int epoll_fd,
    char const* const device,
    bool retry
) {
    if (strncmp(device, "hidraw", strlen("hidraw"))) {
        return 1;
    }

    char path[PATH_MAX + 1] = { 0 };
    if (snprintf(path, PATH_MAX, "/dev/%s", device) >= PATH_MAX) {
        perror("failed to open hidraw device");
        return 1;
    }

    int hidraw_fd = -1;
    // `inotify` can report filesystem events before `udev` has finished
    // applying permissions to the corresponding `hidraw` device. If the
    // device cannot be opened immediately, block and retry. This may
    // cause a noticeable delay when processing game controller input,
    // but it should occur somewhat rarely.
    for (size_t i = 0; i < 10; i++) {
        if (((hidraw_fd = open(path, O_RDONLY | O_NONBLOCK)) != -1) || !retry) {
            break;
        }
        sleep(1);
    }
    if (hidraw_fd == -1) {
        perror("failed to open hidraw device");
        return 1;
    }

    struct hidraw_devinfo info;
    if (ioctl(hidraw_fd, HIDIOCGRAWINFO, &info) < 0) {
        cleanup_fd(&hidraw_fd);
        return 1;
    }

    // 28de:1302 Steam Controller
    // 28de:1304 Steam Controller Puck
    if (!(info.vendor == 0x28de && info.product == 0x1302) &&
        !(info.vendor == 0x28de && info.product == 0x1304)) {
        cleanup_fd(&hidraw_fd);
        return 1;
    }

    int uinput_fd = -1;
    if (virtual_gamepad_setup(&uinput_fd)) {
        cleanup_fd(&hidraw_fd);
        return 1;
    }

    // TODO
    if (epoll_ctl_add(epoll_fd, hidraw_fd, EPOLLIN | EPOLLRDHUP)) {
        cleanup_fd(&hidraw_fd);
        virtual_gamepad_destroy(uinput_fd);
        return 1;
    }

    if (gamepad_slot_insert(slots, device, hidraw_fd, uinput_fd)) {
        epoll_ctl_remove(epoll_fd, hidraw_fd);
        cleanup_fd(&hidraw_fd);
        virtual_gamepad_destroy(uinput_fd);
        return 1;
    }

    printf("added %s...\n", device);
    return 0;
}

int process_gamepad(struct gamepad_slots* slots, int epoll_fd, size_t index) {
    struct gamepad_slot* const slot = &slots->slots[index];

    uint8_t buffer[128] = { 0 };

    ssize_t length = read(slot->hidraw_fd, buffer, sizeof(buffer));
    if (length == -1) {
        // TODO
        if (errno != EIO) {
            perror("failed to read from gamepad");
        }

        if (epoll_ctl_remove(epoll_fd, slot->hidraw_fd)) {
            return 1;
        }
        cleanup_fd(&slot->hidraw_fd);
        virtual_gamepad_destroy(slot->uinput_fd);
        printf("removed %s...\n", slot->device);

        if (gamepad_slot_remove(slots, index)) {
            return 1;
        }

        // TODO
        return 0;
    }

    struct sc_gamepad_state gamepad = { 0 };
    memcpy(&gamepad, &slot->gamepad, sizeof(gamepad));
    sc_gamepad_state_update(&gamepad, buffer);
    sc_gamepad_state_send(slot->uinput_fd, &slot->gamepad, &gamepad);
    //sc_gamepad_state_print(&gamepad);
    memcpy(&slot->gamepad, &gamepad, sizeof(gamepad));

    return 0;
}

#define INOTIFY_BUFFER_SIZE 32 * (sizeof(struct inotify_event) + NAME_MAX)

int process_watched(
    struct gamepad_slots* const slots,
    int epoll_fd,
    int inotify_fd
) {
    // `inotify_event` contains a flexible array member (`name`), which
    // prevents constructing an array of events directly. Instead, this
    // allocates a buffer large enough to hold `MAX_INOTIFY_EVENTS`,
    // each with the maximum possible name length (`NAME_MAX`).
    uint8_t buffer[INOTIFY_BUFFER_SIZE] = { 0 };

    ssize_t length = read(inotify_fd, buffer, INOTIFY_BUFFER_SIZE);
    if (length == -1) {
        perror("failed to read inotify events");
        return 1;
    }

    for (size_t i = 0; i < length; ) {
        struct inotify_event *event = (struct inotify_event*)&buffer[i];

        // When `len` is zero-length, the event applies to the watched
        // directory rather than a specific file within that directory.
        // In this case `name` will not be null terminated, so it
        // cannot be passed into `maybe_add_gamepad`.
        if (event->len) {
            (void)maybe_add_gamepad(slots, epoll_fd, event->name, true);
        }

        // Advance to the next event by skipping the `inotify_event`
        // data and its variable-length name field.
        i += sizeof(struct inotify_event) + event->len;
    }

    return 0;
}

int process_initial(struct gamepad_slots* slots, int epoll_fd) {
    DIR *dir __attribute__((cleanup(cleanup_dir))) = opendir("/dev");
    if (dir == NULL) {
        perror("failed to open /dev directory");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        (void)maybe_add_gamepad(slots, epoll_fd, entry->d_name, false);
    }

    return 0;
}


#define MAX_EPOLL_EVENTS 32

int main (int argc, char** argv) {
    struct gamepad_slots slots = { 0 };

    // This application is structured so that only `epoll_wait` may block.
    // At least one blocking point is necessary to avoid busy-waiting and
    // excessive CPU usage. However, introducing additional blocking
    // points may cause noticeable delays when processing
    // game controller input.
    int epoll_fd __attribute__((cleanup(cleanup_fd))) = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("failed to create epoll file-descriptor");
        return 1;
    }

    int inotify_fd __attribute__((cleanup(cleanup_fd))) = inotify_init();
    if (inotify_fd == -1) {
        perror("failed to create inotify file-descriptor");
        return 1;
    }

    // Watching "/dev" is required to support game controller hot-plugging.
    // Connecting a Steam Controller creates one new "hidraw" device, while
    // connecting a Steam Controller Puck creates four new "hidraw" devices,
    // even if fewer than four controllers are attached to the puck. These
    // devices are filtered and assigned to controller slots later.
    int watchd = inotify_add_watch(inotify_fd, "/dev", IN_CREATE);
    if (watchd == -1) {
        perror("failed to create inotify watch-descriptor");
        return 1;
    }

    // Watch `inotify` for events...
    if (epoll_ctl_add(epoll_fd, inotify_fd, EPOLLIN)) {
        return 1;
    }

    // This performs an initial pass looking for matching "hidraw" devices.
    // This is necessary because the application may start after the Steam
    // Controller or Steam Controller Puck have already been connected.
    //
    // There is a small race window during startup: if either device is
    // connected immediately before application startup, `udev` may not
    // have finished applying permissions to the corresponding "hidraw"
    // device yet. In that case, we will fail to listen to the device
    // for this session, though this situation
    // should be extremely rare.
    if (process_initial(&slots, epoll_fd)) {
        return 1;
    }

    struct epoll_event events[MAX_EPOLL_EVENTS];
    for (;;) {
        // Wait for changes...
        int event_count = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        if (event_count == -1) {
            perror("failed to wait for epoll events");
            return 1;
        }

        // Due to the `epoll` API, there isn't a convenient way to determine
        // if a specific file-descriptor has changed. Instead, this iterates
        // through the events returned by `epoll_wait` and compares each
        // event's file-descriptor against the target descriptors.
        for (size_t i = 0; i < event_count; i++) {
            struct epoll_event *event = &events[i];

            if (inotify_fd == event->data.fd) {
                if (process_watched(&slots, epoll_fd, inotify_fd)) {
                    return 1;
                }
                continue;
            }

            for (size_t j = 0; j < slots.count; j++) {
                struct gamepad_slot* const slot = &slots.slots[j];

                if (slot->hidraw_fd == event->data.fd) {
                    if (process_gamepad(&slots, epoll_fd, j)) {
                        return 1;
                    }
                    break;
                }
            }
        }
    }
}
