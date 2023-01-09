/**
 * File defined by O. Pasquier 5th July 2022
 * Thanks to Microchip

 */
#ifndef CONF_SSD1306_H_INCLUDED
#define CONF_SSD1306_H_INCLUDED

#include <board.h>

#if EXT_USED_SSD1306 == 1
/* Interface configuration for EXT1 SAM Xplained Pro */
#  define SSD1306_SPI                 EXT1_SPI_MODULE
#  define CONFIG_SSD1306_FRAMEBUFFER

#  define SSD1306_DC_PIN              EXT1_PIN_5
#  define SSD1306_RES_PIN             EXT1_PIN_10
#  define SSD1306_CS_PIN              EXT1_PIN_15

#  define SSD1306_SPI_PINMUX_SETTING  EXT1_SPI_SERCOM_MUX_SETTING
#  define SSD1306_SPI_PINMUX_PAD0     EXT1_SPI_SERCOM_PINMUX_PAD0
#  define SSD1306_SPI_PINMUX_PAD1     PINMUX_UNUSED
#  define SSD1306_SPI_PINMUX_PAD2     EXT1_SPI_SERCOM_PINMUX_PAD2
#  define SSD1306_SPI_PINMUX_PAD3     EXT1_SPI_SERCOM_PINMUX_PAD3
#elif EXT_USED_SSD1306 == 2
/* Interface configuration for EXT2 SAM Xplained Pro */
#  define SSD1306_SPI                 EXT2_SPI_MODULE
#  define CONFIG_SSD1306_FRAMEBUFFER

#  define SSD1306_DC_PIN              EXT2_PIN_5
#  define SSD1306_RES_PIN             EXT2_PIN_10
#  define SSD1306_CS_PIN              EXT2_PIN_15

#  define SSD1306_SPI_PINMUX_SETTING  EXT2_SPI_SERCOM_MUX_SETTING
#  define SSD1306_SPI_PINMUX_PAD0     EXT2_SPI_SERCOM_PINMUX_PAD0
#  define SSD1306_SPI_PINMUX_PAD1     PINMUX_UNUSED
#  define SSD1306_SPI_PINMUX_PAD2     EXT2_SPI_SERCOM_PINMUX_PAD2
#  define SSD1306_SPI_PINMUX_PAD3     EXT2_SPI_SERCOM_PINMUX_PAD3
#elif EXT_USED_SSD1306 == 3
/* Interface configuration for EXT3 SAM Xplained Pro */
#  define SSD1306_SPI                 EXT3_SPI_MODULE
#  define CONFIG_SSD1306_FRAMEBUFFER

#  define SSD1306_DC_PIN              EXT3_PIN_5
#  define SSD1306_RES_PIN             EXT3_PIN_10
#  define SSD1306_CS_PIN              EXT3_PIN_15

#  define SSD1306_SPI_PINMUX_SETTING  EXT3_SPI_SERCOM_MUX_SETTING
#  define SSD1306_SPI_PINMUX_PAD0     EXT3_SPI_SERCOM_PINMUX_PAD0
#  define SSD1306_SPI_PINMUX_PAD1     PINMUX_UNUSED
#  define SSD1306_SPI_PINMUX_PAD2     EXT3_SPI_SERCOM_PINMUX_PAD2
#  define SSD1306_SPI_PINMUX_PAD3     EXT3_SPI_SERCOM_PINMUX_PAD3
#else
/* Dummy Interface configuration */
#  define SSD1306_SPI                 0
#  define CONFIG_SSD1306_FRAMEBUFFER

#  define SSD1306_DC_PIN              0
#  define SSD1306_RES_PIN             0
#  define SSD1306_CS_PIN              0

#  define SSD1306_SPI_PINMUX_SETTING  0
#  define SSD1306_SPI_PINMUX_PAD0     0
#  define SSD1306_SPI_PINMUX_PAD1     0
#  define SSD1306_SPI_PINMUX_PAD2     0
#  define SSD1306_SPI_PINMUX_PAD3     0
#endif

// Interface configuration for SAM D21 Xplained Pro
// #  define SSD1306_SPI                 EXT3_SPI_MODULE
// #  define CONFIG_SSD1306_FRAMEBUFFER
// 
// #  define SSD1306_DC_PIN              EXT3_PIN_5
// #  define SSD1306_RES_PIN             EXT3_PIN_10
// #  define SSD1306_CS_PIN              EXT3_PIN_15
// 
// #  define SSD1306_SPI_PINMUX_SETTING  EXT3_SPI_SERCOM_MUX_SETTING
// #  define SSD1306_SPI_PINMUX_PAD0     EXT3_SPI_SERCOM_PINMUX_PAD0
// #  define SSD1306_SPI_PINMUX_PAD1     PINMUX_UNUSED
// #  define SSD1306_SPI_PINMUX_PAD2     EXT3_SPI_SERCOM_PINMUX_PAD2
// #  define SSD1306_SPI_PINMUX_PAD3     EXT3_SPI_SERCOM_PINMUX_PAD3

/* Minimum clock period is 50ns@3.3V -> max frequency is 20MHz */
#define SSD1306_CLOCK_SPEED           1000000UL
#define SSD1306_DISPLAY_CONTRAST_MAX  40
#define SSD1306_DISPLAY_CONTRAST_MIN  30

#endif /* CONF_SSD1306_H_INCLUDED */
