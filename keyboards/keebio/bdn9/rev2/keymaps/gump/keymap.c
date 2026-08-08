// Keebio BDN9 Rev2 keymap (gump).
//
// Physical layout:
//     [L-ENC]      [KEY_TL]     [R-ENC]
//     [KEY_ML]     [KEY_MM]     [KEY_MR]
//     [KEY_BL]     [KEY_BM]     [KEY_BR]
//
// Encoder press is tap-hold (LT):
//   Tap L-ENC  = Mute         Hold L-ENC  = Layer 1 (MEDIA)
//   Tap R-ENC  = Play/Pause   Hold R-ENC  = Layer 2 (RGB)
//   Hold BOTH  = Layer 3 (SYSTEM) via update_tri_layer_state()
//
// Encoder rotation is layer-aware — see encoder_update_user() below.
#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _MEDIA,
    _RGB,
    _SYSTEM,
};

enum encoder_names {
    _LEFT,
    _RIGHT,
    _MIDDLE,  // unused on 2-encoder boards
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* BASE */
    [_BASE] = LAYOUT(
        LT(_MEDIA, KC_MUTE), KC_HOME, LT(_RGB, KC_MPLY),
        KC_APP,              KC_UP,   RM_TOGG,
        KC_LEFT,             KC_DOWN, KC_RGHT
    ),

    /* MEDIA — hold left encoder */
    [_MEDIA] = LAYOUT(
        _______, KC_END,  _______,
        _______, KC_PGUP, _______,
        KC_MPRV, KC_PGDN, KC_MNXT
    ),

    /* RGB — hold right encoder */
    [_RGB] = LAYOUT(
        _______, RM_HUEU, _______,
        RM_VALD, RM_SATU, RM_VALU,
        RM_HUED, RM_SATD, RM_NEXT
    ),

    /* SYSTEM — hold BOTH encoders (auto via tri-layer) */
    [_SYSTEM] = LAYOUT(
        QK_BOOT, _______, EE_CLR,
        _______, RM_SPDU, _______,
        _______, RM_SPDD, RM_PREV
    ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _MEDIA, _RGB, _SYSTEM);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case _BASE:
            if (index == _LEFT)  tap_code(clockwise ? KC_VOLU : KC_VOLD);
            if (index == _RIGHT) tap_code(clockwise ? KC_PGDN : KC_PGUP);
            break;
        case _MEDIA:
            if (index == _LEFT)  tap_code(clockwise ? KC_MNXT : KC_MPRV);
            if (index == _RIGHT) tap_code(clockwise ? KC_WH_D : KC_WH_U);
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

