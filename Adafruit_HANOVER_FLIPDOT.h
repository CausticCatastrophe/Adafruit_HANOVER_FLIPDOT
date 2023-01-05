/*!
 * @file Adafruit_HANOVER_FLIPDOT.h
 *
 * This is part of for Adafruit's HANOVER_FLIPDOT library for monochrome
 * OLED displays: http://www.adafruit.com/category/63_98
 *
 * These displays use I2C or SPI to communicate. I2C requires 2 pins
 * (SCL+SDA) and optionally a RESET pin. SPI requires 4 pins (MOSI, SCK,
 * select, data/command) and optionally a reset pin. Hardware SPI or
 * 'bitbang' software SPI are both supported.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.
 *
 * BSD license, all text above, and the splash screen header file,
 * must be included in any redistribution.
 *
 */

#ifndef _Adafruit_HANOVER_FLIPDOT_H_
#define _Adafruit_HANOVER_FLIPDOT_H_


// Uncomment to disable Adafruit splash logo
#define HANOVER_FLIPDOT_NO_SPLASH

#include <Adafruit_GFX.h>

/// The following "raw" color names are kept for backwards client compatability
/// They can be disabled by predefining this macro before including the Adafruit
/// header client code will then need to be modified to use the scoped enum
/// values directly
#ifndef NO_ADAFRUIT_HANOVER_FLIPDOT_COLOR_COMPATIBILITY
#define BLACK HANOVER_FLIPDOT_BLACK     ///< Draw 'off' pixels
#define WHITE HANOVER_FLIPDOT_YELLOW     ///< Draw 'on' pixels
#define INVERSE HANOVER_FLIPDOT_INVERSE ///< Invert pixels
#endif
/// fit into the HANOVER_FLIPDOT_ naming scheme
#define HANOVER_FLIPDOT_BLACK 0   ///< Draw 'off' pixels
#define HANOVER_FLIPDOT_YELLOW 1   ///< Draw 'on' pixels
#define HANOVER_FLIPDOT_INVERSE 2 ///< Invert pixels

#define HANOVER_FLIPDOT_MEMORYMODE 0x20          ///< See datasheet
#define HANOVER_FLIPDOT_COLUMNADDR 0x21          ///< See datasheet
#define HANOVER_FLIPDOT_PAGEADDR 0x22            ///< See datasheet
#define HANOVER_FLIPDOT_SETCONTRAST 0x81         ///< See datasheet
#define HANOVER_FLIPDOT_CHARGEPUMP 0x8D          ///< See datasheet
#define HANOVER_FLIPDOT_SEGREMAP 0xA0            ///< See datasheet
#define HANOVER_FLIPDOT_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define HANOVER_FLIPDOT_DISPLAYALLON 0xA5        ///< Not currently used
#define HANOVER_FLIPDOT_NORMALDISPLAY 0xA6       ///< See datasheet
#define HANOVER_FLIPDOT_INVERTDISPLAY 0xA7       ///< See datasheet
#define HANOVER_FLIPDOT_SETMULTIPLEX 0xA8        ///< See datasheet
#define HANOVER_FLIPDOT_DISPLAYOFF 0xAE          ///< See datasheet
#define HANOVER_FLIPDOT_DISPLAYON 0xAF           ///< See datasheet
#define HANOVER_FLIPDOT_COMSCANINC 0xC0          ///< Not currently used
#define HANOVER_FLIPDOT_COMSCANDEC 0xC8          ///< See datasheet
#define HANOVER_FLIPDOT_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define HANOVER_FLIPDOT_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define HANOVER_FLIPDOT_SETPRECHARGE 0xD9        ///< See datasheet
#define HANOVER_FLIPDOT_SETCOMPINS 0xDA          ///< See datasheet
#define HANOVER_FLIPDOT_SETVCOMDETECT 0xDB       ///< See datasheet

#define HANOVER_FLIPDOT_SETLOWCOLUMN 0x00  ///< Not currently used
#define HANOVER_FLIPDOT_SETHIGHCOLUMN 0x10 ///< Not currently used
#define HANOVER_FLIPDOT_SETSTARTLINE 0x40  ///< See datasheet

#define HANOVER_FLIPDOT_EXTERNALVCC 0x01  ///< External display voltage source
#define HANOVER_FLIPDOT_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define HANOVER_FLIPDOT_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define HANOVER_FLIPDOT_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define HANOVER_FLIPDOT_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define HANOVER_FLIPDOT_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define HANOVER_FLIPDOT_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define HANOVER_FLIPDOT_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define HANOVER_FLIPDOT_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

/*!
    @brief  Class that stores state and functions for interacting with
            HANOVER_FLIPDOT OLED displays.
*/
class Adafruit_HANOVER_FLIPDOT : public Adafruit_GFX {
public:
  // NEW CONSTRUCTORS -- recommended for new projects
  Adafruit_HANOVER_FLIPDOT(uint8_t w, uint8_t h, int8_t reset_pin, int8_t row_adv_pin, int8_t col_adv_pin, int8_t coil_pulse_pin, int8_t set_pin, int8_t disp1_enable_pin, int8_t disp2_enable_pin, int8_t disp3_enable_pin, int8_t disp4_enable_pin);

  ~Adafruit_HANOVER_FLIPDOT(void);

  bool begin(bool reset = true, uint8_t display_idx = 1);
  void display(void);
  void clearDisplay(void);
  void invertDisplay(bool i);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  bool getPixel(int16_t x, int16_t y);
  uint8_t *getBuffer(void);

protected:
  inline void SPIwrite(uint8_t d) __attribute__((always_inline));
  void HANOVER_FLIPDOT_command1(uint8_t c);
  void HANOVER_FLIPDOT_commandList(const uint8_t *c, uint8_t n);

  uint8_t *buffer; ///< Buffer data used for display buffer. Allocated when begin method is called.

  int8_t reset_pin;          ///< The pin used to reset both row and col binary counters. Set during construction.
  int8_t row_adv_pin;        ///< The pin used to advance the row. Set during construction.
  int8_t col_adv_pin;        ///< The pin used to advance the col. Set during construction.
  int8_t coil_pulse_pin;     ///< Used to provide drive power to change the dot state. Set during construction.
  int8_t set_pin;            ///< Used to select which way the current will be pulsed. Set during construction.
  int8_t disp1_enable_pin;   ///< The pin used to select display 1. Set during construction.
  int8_t disp2_enable_pin;   ///< The pin used to select display 2. Set during construction.
  int8_t disp3_enable_pin;   ///< The pin used to select display 3. Set during construction.
  int8_t disp4_enable_pin;   ///< The pin used to select display 4. Set during construction.
