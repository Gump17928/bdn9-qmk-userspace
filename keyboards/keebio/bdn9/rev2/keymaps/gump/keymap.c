// MINIMAL diagnostic keymap.  No VIA, no tap dance, no encoders, no
// indicators, no init hooks.  If LEDs still show stuck white with this,
// the problem is the WS2812 driver/config, not our code.
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_MUTE, KC_HOME, KC_MPLY,
        KC_A,    KC_UP,   KC_B,
        KC_LEFT, KC_DOWN, KC_RGHT
    ),
};
