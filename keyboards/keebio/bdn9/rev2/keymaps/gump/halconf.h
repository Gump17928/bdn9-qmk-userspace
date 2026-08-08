// Override the keyboard-level halconf.h to enable SPI (was PWM only).
#pragma once

#define HAL_USE_SPI TRUE

#include_next <halconf.h>
