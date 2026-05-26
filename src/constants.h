#ifndef SCWRAPPER_CONSTANTS_H
#define SCWRAPPER_CONSTANTS_H

#include <stdint.h>
#include <linux/input-event-codes.h>

// Steam vendor/product IDs
#define STEAM_VENDOR_ID 0x28de
#define STEAM_PRODUCT_CONTROLLER 0x1302
#define STEAM_PRODUCT_CONTROLLER_BLUETOOTH 0x1303
#define STEAM_PRODUCT_PUCK 0x1304

// Virtual gamepad metadata
#define VIRTUAL_GAMEPAD_NAME "Virtual Controller"
#define VIRTUAL_GAMEPAD_VENDOR 0x0000
#define VIRTUAL_GAMEPAD_PRODUCT 0x0000
#define VIRTUAL_GAMEPAD_VERSION 1

// Virtual mouse metadata
#define VIRTUAL_MOUSE_NAME "Virtual Mouse"
#define VIRTUAL_MOUSE_VENDOR 0x0000
#define VIRTUAL_MOUSE_PRODUCT 0x0000
#define VIRTUAL_MOUSE_VERSION 1

// Gamepad buttons
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

// Gamepad axis limits
#define HAT_MIN -1
#define HAT_MAX 1
#define TRIGGER_MIN 0
#define TRIGGER_MAX 32767
#define THUMBSTICK_MIN -32767
#define THUMBSTICK_MAX 32767
#define TOUCHPAD_MIN -32767
#define TOUCHPAD_MAX 32767

#endif // SCWRAPPER_CONSTANTS_H
