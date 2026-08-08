// Override the keyboard-level mcuconf.h to also enable SPI2.
#pragma once

#include_next <mcuconf.h>

#undef STM32_SPI_USE_SPI2
#define STM32_SPI_USE_SPI2 TRUE
