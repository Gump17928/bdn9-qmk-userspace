#pragma once

// WS2812 via SPI2 on STM32F072.  Pin B15 (declared in keyboard.json)
// is SPI2_MOSI at alternate function 0.  SCK pin is unused by us but
// the F072 SPI peripheral requires one wired up — B13 is SPI2_SCK AF0
// and isn't used by matrix or encoders on the BDN9 Rev2.
#define WS2812_SPI_DRIVER       SPID2
#define WS2812_SPI_MOSI_PAL_MODE 0
#define WS2812_SPI_SCK_PIN      B13
#define WS2812_SPI_SCK_PAL_MODE 0
