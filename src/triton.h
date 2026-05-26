#ifndef SCWRAPPER_TRITON_H
#define SCWRAPPER_TRITON_H

#include "sc_haptics.h"

#include <stdint.h>

#define TRITON_ID_IN_CONTROLLER_STATE           0x42
#define TRITON_ID_IN_BATTERY_STATUS             0x43
#define TRITON_ID_IN_CONTROLLER_STATE_BLE       0x45
#define TRITON_ID_IN_WIRELESS_STATUS_X          0x46
#define TRITON_ID_IN_CONTROLLER_STATE_TIMESTAMP 0x47
#define TRITON_ID_IN_WIRELESS_STATUS            0x79

#define TRITON_ID_OUT_HAPTICS_RUMBLE    0x80
#define TRITON_ID_OUT_HAPTICS_PULSE     0x81
#define TRITON_ID_OUT_HAPTICS_COMMAND   0x82
#define TRITON_ID_OUT_HAPTICS_LFO_TONE  0x83
#define TRITON_ID_OUT_HAPTICS_LOG_SWEEP 0x84
#define TRITON_ID_OUT_HAPTICS_SCRIPT    0x85

#define TRITON_ID_SET_DIGITAL_MAPPINGS         0x80
#define TRITON_ID_CLEAR_DIGITAL_MAPPINGS       0x81
#define TRITON_ID_GET_DIGITAL_MAPPINGS         0x82
#define TRITON_ID_GET_ATTRIBUTE_VALUES         0x83
#define TRITON_ID_GET_ATTRIBUTE_LABEL          0x84
#define TRITON_ID_SET_DEFAULT_DIGITAL_MAPPINGS 0x85
#define TRITON_ID_FACTORY_RESET                0x86
#define TRITON_ID_SET_SETTINGS_VALUES          0x87
#define TRITON_ID_CLEAR_SETTINGS_VALUES        0x88
#define TRITON_ID_GET_SETTINGS_VALUES          0x89
#define TRITON_ID_GET_SETTINGS_LABEL           0x8A
#define TRITON_ID_GET_SETTINGS_MAXS            0x8B
#define TRITON_ID_SET_CONTOLLER_MODE           0x8D
#define TRITON_ID_LOAD_DEFAULT_SETTINGS        0x8E
#define TRITON_ID_HAPTIC_PULSE                 0x8F

#define TRITON_HAPTICS_LFO_TONE_LRA_L    0x00
#define TRITON_HAPTICS_LFO_TONE_LRA_R    0x01
#define TRITON_HAPTICS_LFO_TONE_LRA_BOTH 0x02
#define TRITON_HAPTICS_LFO_TONE_HI_LRA_L 0x03
#define TRITON_HAPTICS_LFO_TONE_HI_LRA_R 0x04

#define TRITON_SETTING_LIZARD_MODE 0x09

int triton_disable_lizard_mode(int fd);
int triton_clear_digital_mappings(int fd);
int triton_haptics_rumble(int fd, struct ScHapticsRumble rumble);
int triton_haptics_lfo_tone(int fd, struct ScHapticsLfoTone lfo_tone);

#endif // SCWRAPPER_TRITON_H
