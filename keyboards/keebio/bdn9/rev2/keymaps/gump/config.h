#pragma once

// WS2812 via SPI2 on STM32F072.  Pin B15 (declared in keyboard.json)
// is SPI2_MOSI at alternate function 0.  SCK pin is unused by us but
// the F072 SPI peripheral requires one wired up — B13 is SPI2_SCK AF0
// and isn't used by matrix or encoders on the BDN9 Rev2.
#define WS2812_SPI_DRIVER       SPID2
#define WS2812_SPI_MOSI_PAL_MODE 0
#define WS2812_SPI_SCK_PIN      B13
#define WS2812_SPI_SCK_PAL_MODE 0

// The underglow LEDs are painfully bright at 255.  Cap and default low.
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150
#define RGB_MATRIX_DEFAULT_VAL       60
#define RGB_MATRIX_DEFAULT_MODE      RGB_MATRIX_BREATHING
#define RGB_MATRIX_DEFAULT_HUE       128
#define RGB_MATRIX_DEFAULT_SAT       255

// VIA reserves EEPROM per remappable layer.  We ship 6 (BASE, MEDIA,
// RGB, SYSTEM, MACRO, VIA_MACROS) so bump from the default of 4.
#define DYNAMIC_KEYMAP_LAYER_COUNT 6

// Reserve EEPROM for our persistent on-pad macro storage
// (pm_persist_t = 2 * (64 keys + 1 count + pad) = 132 bytes; round up).
#define EECONFIG_USER_DATA_SIZE 140
