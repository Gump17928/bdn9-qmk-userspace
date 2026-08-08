# RGB Matrix is declared as a feature in keyboard.json, but the data-driven
# enable doesn't always propagate the RGB_MATRIX_ENABLE define reliably.
# Enable it explicitly so the driver, effects, and RM_* keycodes all compile in.
RGB_MATRIX_ENABLE = yes

# Do NOT also enable RGBLIGHT — it fights with RGB Matrix over the same
# WS2812 chain and causes weird behavior / stuck full-brightness white.

# VIA runtime remapping + macro editor (usevia.app). Persists to EEPROM emul.
VIA_ENABLE = yes

# Tap dance drives the layer-cycle key on the top-middle position.
TAP_DANCE_ENABLE = yes

# Mouse keys enable KC_WH_U / KC_WH_D used for encoder scroll on MEDIA layer.
MOUSEKEY_ENABLE = yes

# Encoders are enabled by default in keyboard.json; nothing to do here.
