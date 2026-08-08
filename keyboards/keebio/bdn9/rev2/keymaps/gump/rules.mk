# WS2812 via SPI2 — B15=MOSI on STM32F072.  Bitbang was silently broken.
RGB_MATRIX_ENABLE = yes
WS2812_DRIVER = spi

# Tap dance drives the layer-cycle key on the top-middle position.
TAP_DANCE_ENABLE = yes

# Mouse keys enable MS_WHLU / MS_WHLD used for encoder scroll on MEDIA layer.
MOUSEKEY_ENABLE = yes

# VIA runtime remapping + macro editor at usevia.app.  Persists to EEPROM.
VIA_ENABLE = yes

# Persistent on-pad macro recorder is implemented in keymap.c —
# QMK Dynamic Macros are ephemeral, so we roll our own.
