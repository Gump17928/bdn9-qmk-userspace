# Attempt #2: switch WS2812 from default bitbang to SPI driver.
# B15 = SPI2_MOSI on STM32F072, so this is the natural fit and
# more reliable than the default bitbang timing.
RGB_MATRIX_ENABLE = yes
WS2812_DRIVER = spi
