// BDN9 Rev2 keymap (gump).  WS2812 uses SPI2 driver — see rules.mk.
#include QMK_KEYBOARD_H
#include <string.h>

enum layers {
    _BASE = 0,
    _MEDIA,
    _RGB,
    _SYSTEM,
    _MACRO,       // on-pad recorded macros, persistent via EEPROM
    _VIA_MACROS,  // VIA-programmed macros, edited via usevia.app
};

enum encoder_names {
    _LEFT,
    _RIGHT,
    _MIDDLE,
};

enum custom_keycodes {
    PM_REC1 = SAFE_RANGE,  // start recording into persistent slot 1
    PM_REC2,
    PM_STOP,               // stop + write current slot to EEPROM
    PM_PLAY1,
    PM_PLAY2,
};

enum tap_dance_ids {
    TD_LAYER_CYCLE,
};

// -------- Persistent on-pad macro storage --------
#define PM_SLOTS      2
#define PM_SLOT_LEN   32          // max keystrokes captured per slot

typedef struct {
    uint16_t keys[PM_SLOT_LEN];
    uint8_t  count;
} pm_slot_t;

typedef struct {
    pm_slot_t slots[PM_SLOTS];
} pm_persist_t;

static pm_persist_t pm_data;
static int8_t       pm_recording_slot = -1;  // -1 = idle

// -------- Tap dance: cycle layers on the top-middle key --------
static void td_layer_cycle_finished(tap_dance_state_t *state, void *user_data) {
    if (get_highest_layer(layer_state) != _BASE) {
        layer_clear();
        return;
    }
    switch (state->count) {
        case 1:  layer_move(_MEDIA);       break;
        case 2:  layer_move(_RGB);         break;
        case 3:  layer_move(_SYSTEM);      break;
        case 4:  layer_move(_MACRO);       break;
        default: layer_move(_VIA_MACROS);  break;
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
    // Persistent on-pad recorder.  Record→BASE type your macro→come back
    // and STOP (which writes to EEPROM).  Play from anywhere via PM_PLYn.
    [_MACRO] = LAYOUT(
        PM_STOP, _______, _______,
        PM_REC1, _______, PM_PLAY1,
        PM_REC2, _______, PM_PLAY2
    ),
    // VIA-programmed macros; use usevia.app to author them.
    [_VIA_MACROS] = LAYOUT(
        _______,    _______,       _______,
        MC_1,       MC_2,          MC_3,
        MC_4,       MC_5,          MC_6
    ),
};


// -------- Encoders (layer-aware) --------
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

// -------- Layer indicator + record-blink on top-middle LED --------
#define LAYER_INDICATOR_LED 1

bool rgb_matrix_indicators_user(void) {
    // While recording, override with a fast red blink — takes priority
    // over the normal layer color.
    if (pm_recording_slot >= 0) {
        uint16_t t = timer_read() % 400;
        uint8_t on = (t < 200) ? 255 : 0;
        rgb_matrix_set_color(LAYER_INDICATOR_LED, on, 0, 0);
        return false;
    }

    uint8_t layer = get_highest_layer(layer_state);
    if (layer == _BASE) return false;

    uint8_t r = 0, g = 0, b = 0;
    switch (layer) {
        case _MEDIA:      g = 255;             break;
        case _RGB:        b = 255;             break;
        case _SYSTEM:     r = 255;             break;
        case _MACRO:      r = 255; b = 255;    break;  // magenta
        case _VIA_MACROS: r = 255; g = 255;    break;  // yellow
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

// Only capture "real" HID keystrokes during recording; skip our own
// control keycodes, layer switches, macros, etc.
static bool pm_is_recordable(uint16_t keycode) {
    if (keycode >= KC_A && keycode <= KC_EXSEL)             return true;
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) return true;
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case PM_REC1:
            if (record->event.pressed) {
                pm_recording_slot = 0;
                pm_data.slots[0].count = 0;
                layer_move(_BASE);   // drop back so user records BASE keys
            }
            return false;
        case PM_REC2:
            if (record->event.pressed) {
                pm_recording_slot = 1;
                pm_data.slots[1].count = 0;
                layer_move(_BASE);
            }
            return false;
        case PM_STOP:
            if (record->event.pressed && pm_recording_slot >= 0) {
                eeconfig_update_user_datablock(&pm_data, 0, sizeof(pm_data));
                pm_recording_slot = -1;
            }
            return false;
        case PM_PLAY1:
            if (record->event.pressed) {
                pm_slot_t *s = &pm_data.slots[0];
                for (uint8_t i = 0; i < s->count; i++) tap_code16(s->keys[i]);
            }
            return false;
        case PM_PLAY2:
            if (record->event.pressed) {
                pm_slot_t *s = &pm_data.slots[1];
                for (uint8_t i = 0; i < s->count; i++) tap_code16(s->keys[i]);
            }
            return false;
        default:
            if (pm_recording_slot >= 0 && record->event.pressed) {
                // Skip modifier keys — we bake their state into the next
                // captured basic keycode so replay reproduces Shift+A etc.
                if (IS_MODIFIER_KEYCODE(keycode)) return true;
                if (!pm_is_recordable(keycode))  return true;

                uint16_t code = keycode;
                uint8_t  mods = get_mods();
                if (mods & MOD_MASK_SHIFT) code = LSFT(code);
                if (mods & MOD_MASK_CTRL)  code = LCTL(code);
                if (mods & MOD_MASK_ALT)   code = LALT(code);
                if (mods & MOD_MASK_GUI)   code = LGUI(code);

                pm_slot_t *s = &pm_data.slots[pm_recording_slot];
                if (s->count < PM_SLOT_LEN) s->keys[s->count++] = code;
            }
            return true;
    }
}

void keyboard_post_init_user(void) {
    if (!eeconfig_is_user_datablock_valid()) {
        eeconfig_init_user_datablock();
    }
    eeconfig_read_user_datablock(&pm_data, 0, sizeof(pm_data));
}
