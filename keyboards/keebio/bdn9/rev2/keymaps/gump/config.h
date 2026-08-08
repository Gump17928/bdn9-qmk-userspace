// Keymap-level overrides for Keebio BDN9 Rev2 (gump).
#pragma once

// VIA reserves EEPROM for a fixed number of remappable layers (default 4).
// We ship 4 (BASE, MEDIA, RGB, SYSTEM), so keep this at 4.
#define DYNAMIC_KEYMAP_LAYER_COUNT 4

// Cap RGB Matrix brightness — the underglow LEDs are painfully bright by
// default.  0-255; 60 is a comfortable desk level, 150 is the max we allow.
#define RGB_MATRIX_DEFAULT_VAL 60
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150

// Pick a mode that's visibly ALIVE on fresh EEPROM.  If the LEDs still show
// solid white after EE_CLR, RGB Matrix isn't running.  Cyan breathing = clearly
// dynamic, so we can tell at a glance whether the driver is doing its job.
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_BREATHING
#define RGB_MATRIX_DEFAULT_HUE  128
#define RGB_MATRIX_DEFAULT_SAT  255
