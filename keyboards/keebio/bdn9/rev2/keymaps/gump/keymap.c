// Ported from the upstream `keebio/bdn9/keymaps/default/keymap.c` as a starting
// point.  Once flashed with VIA enabled, further changes can be made live at
// https://usevia.app without recompiling.
#include QMK_KEYBOARD_H

enum encoder_names {
    _LEFT,
    _RIGHT,
    _MIDDLE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0
     *   Knob1: Vol-/Vol+      Home            Knob3: PgDn/PgUp
     *   Press: Mute           Up              Press: Play/Pause
     *   Hold MO(1)                            RM_NEXT
     *   Left                  Down            Right
     */
    [0] = LAYOUT(
        KC_MUTE, KC_HOME, KC_MPLY,
        MO(1),   KC_UP,   RM_NEXT,
        KC_LEFT, KC_DOWN, KC_RGHT
    ),

    /* Layer 1 (hold top-left) */
    [1] = LAYOUT(
        QK_BOOT, BL_STEP, KC_STOP,
        _______, KC_HOME, RM_NEXT,
        KC_MPRV, KC_END,  KC_MNXT
    ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        tap_code(clockwise ? KC_VOLU : KC_VOLD);
    } else if (index == _MIDDLE) {
        tap_code(clockwise ? KC_DOWN : KC_UP);
    } else if (index == _RIGHT) {
        tap_code(clockwise ? KC_PGDN : KC_PGUP);
    }
    return false;
}
