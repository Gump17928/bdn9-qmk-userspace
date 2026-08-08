// Keebio BDN9 Rev2 keymap (gump).
//
// Physical layout:
//     [L-ENC]      [KEY_TL]     [R-ENC]
//     [KEY_ML]     [KEY_MM]     [KEY_MR]
//     [KEY_BL]     [KEY_BM]     [KEY_BR]
//
// Layer switching: tap the top-middle key (KEY_TL) a number of times.
//   From BASE:   1 tap  -> MEDIA         2 taps -> RGB        3 taps -> SYSTEM
//   From any:    any # of taps -> back to BASE (sticky layers).
//
// Encoder presses stay plain Mute / Play/Pause.
// Encoder rotation is layer-aware — see encoder_update_user().
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

enum tap_dance_ids {
    TD_LAYER_CYCLE = 0,
};

// Fires once per tap-dance sequence, after TAPPING_TERM elapses from last tap.
static void td_layer_cycle_finished(tap_dance_state_t *state, void *user_data) {
    if (get_highest_layer(layer_state) != _BASE) {
        // On any non-base layer, any tap count returns to base.
        layer_clear();
        return;
    }
    switch (state->count) {
        case 1: layer_on(_MEDIA);  break;
        case 2: layer_on(_RGB);    break;
        case 3: layer_on(_SYSTEM); break;
        default: break;  // 4+ taps: no-op, protects against accidental mashing
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_LAYER_CYCLE] = ACTION_TAP_DANCE_FN(td_layer_cycle_finished),
};

#define LAYER_TD TD(TD_LAYER_CYCLE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* BASE — top-middle cycles layers by tap count.
       TEMP: middle-left is EE_CLR while we debug stuck-white RGB.  Press
       once to wipe EEPROM; will be restored to KC_APP once RGB works. */
    [_BASE] = LAYOUT(
        KC_MUTE, LAYER_TD, KC_MPLY,
        EE_CLR,  KC_UP,    RM_TOGG,
        KC_LEFT, KC_DOWN,  KC_RGHT
    ),

    /* MEDIA — 1 tap from base.  Top-middle transparent so tap dance still
       runs to return to base. */
    [_MEDIA] = LAYOUT(
        _______, _______, _______,
        KC_HOME, KC_PGUP, KC_END,
        KC_MPRV, KC_PGDN, KC_MNXT
    ),

    /* RGB — 2 taps from base. */
    [_RGB] = LAYOUT(
        _______, _______, _______,
        RM_VALD, RM_SATU, RM_VALU,
        RM_HUED, RM_SATD, RM_NEXT
    ),

    /* SYSTEM — 3 taps from base. */
    [_SYSTEM] = LAYOUT(
        QK_BOOT, _______, EE_CLR,
        _______, RM_SPDU, _______,
        _______, RM_SPDD, RM_PREV
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

// LED index of the top-middle key (matches info.json ordering).
#define LAYER_INDICATOR_LED 1

// Slow-pulse the top-middle key when a non-BASE layer is active.  Colors:
//   MEDIA  = green, RGB = blue, SYSTEM = red.  Overrides whatever the base
//   RGB Matrix effect wanted to show on that one LED.
bool rgb_matrix_indicators_user(void) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer == _BASE) return false;

    uint8_t r = 0, g = 0, b = 0;
    switch (layer) {
        case _MEDIA:  g = 255; break;
        case _RGB:    b = 255; break;
        case _SYSTEM: r = 255; break;
        default: return false;
    }

    // Triangle wave 0 -> 255 -> 0 over 2 s so it fades in and out gently.
    uint16_t t = timer_read() % 2000;
    uint8_t  phase = (t < 1000) ? (t * 255 / 1000) : ((2000 - t) * 255 / 1000);

    rgb_matrix_set_color(LAYER_INDICATOR_LED,
                         (uint16_t)r * phase / 255,
                         (uint16_t)g * phase / 255,
                         (uint16_t)b * phase / 255);
    return false;
}


