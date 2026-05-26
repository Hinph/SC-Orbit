#ifndef SCWRAPPER_SC_HAPTICS_H
#define SCWRAPPER_SC_HAPTICS_H

#include <stdint.h>

struct ScHapticsRumble {
    uint16_t strong;
    uint16_t weak;
};

#define SC_HAPTICS_LFO_TONE_CHANNEL_LRA_L    0
#define SC_HAPTICS_LFO_TONE_CHANNEL_LRA_R    1
#define SC_HAPTICS_LFO_TONE_CHANNEL_LRA_BOTH 2
#define SC_HAPTICS_LFO_TONE_CHANNEL_HI_LRA_L 3
#define SC_HAPTICS_LFO_TONE_CHANNEL_HI_LRA_R 4

struct ScHapticsLfoTone {
    uint8_t  channel;
    int8_t   gain;
    uint16_t frequency;
    uint16_t duration_ms;
    uint16_t lfo_frequency;
    uint8_t  lfo_depth;
};

#define SC_HAPTICS_ID_NONE     0
#define SC_HAPTICS_ID_RUMBLE   1
#define SC_HAPTICS_ID_LFO_TONE 2

struct ScHaptics {
    int32_t id;
    union {
        struct ScHapticsRumble  rumble;
        struct ScHapticsLfoTone lfo_tone;
    } u;
};

#endif
