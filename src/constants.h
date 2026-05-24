#ifndef SCWRAPPER_CONSTANTS_H
#define SCWRAPPER_CONSTANTS_H

#include <stdint.h>

// Steam vendor/product IDs
#define STEAM_VENDOR_ID 0x28de
#define STEAM_PRODUCT_CONTROLLER 0x1302
#define STEAM_PRODUCT_CONTROLLER_BLUETOOTH 0x1303
#define STEAM_PRODUCT_PUCK 0x1304

// Device name buffer
#define DEVICE_NAME_BUF 256

// HID feature/report constants used when configuring the controller
#define REPORT_ID_1 0x01
#define ID_SET_SETTINGS_VALUES 0x87
#define ID_CLEAR_DIGITAL_MAPPINGS 0x81
#define SETTING_LIZARD_MODE 0x09
#define LENGTH_3 0x03
#define LENGTH_0 0x00

// Virtual device metadata
#define VIRTUAL_DEV_NAME_GAMEPAD "Virtual Controller"
#define VIRTUAL_DEV_VENDOR 0x0000
#define VIRTUAL_DEV_PRODUCT 0x0000
#define VIRTUAL_DEV_VERSION 1

#define VIRTUAL_MOUSE_NAME "Virtual Mouse"
#define VIRTUAL_MOUSE_VENDOR 0x0000
#define VIRTUAL_MOUSE_PRODUCT 0x0000
#define VIRTUAL_MOUSE_VERSION 1

#endif // SCWRAPPER_CONSTANTS_H
