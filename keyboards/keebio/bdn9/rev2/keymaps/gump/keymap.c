// BDN9 Rev2 keymap (gump).  WS2812 uses SPI2 driver — see rules.mk.
#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _MEDIA,
    _RGB,
    _SYSTEM,
    _MACRO,
};

enum encoder_names {
    _LEFT,
    _RIGHT,
    _MIDDLE,
};

enum tap_dance_ids {
    TD_LAYER_CYCLE,
};

// Tap = advance to next non-base layer; when on any non-base layer, a
// single tap returns to BASE.  1=MEDIA, 2=RGB, 3=SYSTEM, 4=MACRO.
static void td_layer_cycle_finished(tap_dance_state_t *state, void *user_data) {
    if (get_highest_layer(layer_state) != _BASE) {
        layer_clear();
        return;
    }
    switch (state->count) {
        case 1:  layer_move(_MEDIA);  break;
        case 2:  layer_move(_RGB);    break;
        case 3:  layer_move(_SYSTEM); break;
        default: layer_move(_MACRO);  break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_LAYER_CYCLE] = ACTION_TAP_DANCE_FN(td_layer_cycle_finished),
};

#define LAYER_TD TD(TD_LAYER_CYCLE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_MUTE, LAYER_TD, KC_MPLY,
        KC_APP,  KC_UP,    RM_TOGG,
        KC_LEFT, KC_DOWN,  KC_RGHT
    ),
    [_MEDIA] = LAYOUT(
        _______, _______, _______,
        KC_HOME, KC_PGUP, KC_END,
        KC_MPRV, KC_PGDN, KC_MNXT
    ),
    [_RGB] = LAYOUT(
        _______, _______, _______,
        RM_VALD, RM_SATU, RM_VALU,
        RM_HUED, RM_SATD, RM_NEXT
    ),
    [_SYSTEM] = LAYOUT(
        QK_BOOT, _______, EE_CLR,
        _______, RM_SPDU, _______,
        _______, RM_SPDD, RM_PREV
    ),
    // MACRO layer — record/play macros directly on the pad.
    // Top-left = stop recording.  Middle/bottom-left record into slots
    // 1 and 2; middle/bottom-right play them back.  Top-right cancels a
    // recording in progress.
    [_MACRO] = LAYOUT(
        DM_RSTP, _______, DM_RTOG,
        DM_REC1, _______, DM_PLY1,
        DM_REC2, _______, DM_PLY2
    ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case _BASE:
            if (index == _LEFT)  tap_code(clockwise ? KC_VOLU : KC_VOLD);
            if (index == _RIGHT) tap_code(clockwise ? KC_PGDN : KC_PGUP);
            break;
        case _MEDIA:
            if (index == _LEFT)  tap_code(clockwise ? KC_MNXT : KC_MPRV);
            if (index == _RIGHT) tap_code(clockwise ? MS_WHLD : MS_WHLU);
            break;
        case _RGB:
            if (index == _LEFT)  tap_code16(clockwise ? RM_VALU : RM_VALD);
            if (index == _RIGHT) tap_code16(clockwise ? RM_HUEU : RM_HUED);
            break;
        case _SYSTEM:
            if (index == _LEFT)  tap_code16(clockwise ? RM_SPDU : RM_SPDD);
            if (index == _RIGHT) tap_code16(clockwise ? RM_NEXT : RM_PREV);
            break;
    }
    return false;
}

// LED index of the top-middle key — matches keyboard.json ordering.
#define LAYER_INDICATOR_LED 1

// Pulse the top-middle key when a non-BASE layer is active.
// MEDIA=green, RGB=blue, SYSTEM=red, MACRO=magenta.  Triangle wave over 2s.
bool rgb_matrix_indicators_user(void) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer == _BASE) return false;

    uint8_t r = 0, g = 0, b = 0;
    switch (layer) {
        case _MEDIA:  g = 255;             break;
        case _RGB:    b = 255;             break;
        case _SYSTEM: r = 255;             break;
        case _MACRO:  r = 255; b = 255;    break;
        default: return false;
    }

    uint16_t t = timer_read() % 2000;
    uint8_t phase = (t < 1000) ? (t * 255 / 1000) : ((2000 - t) * 255 / 1000);
    rgb_matrix_set_color(LAYER_INDICATOR_LED,
                         (r * phase) / 255,
                         (g * phase) / 255,
                         (b * phase) / 255);
    return false;
}
