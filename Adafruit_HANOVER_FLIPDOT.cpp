/*!
 * @file Adafruit_HANOVER_FLIPDOT.cpp
 *
 * @mainpage Arduino library for HANOVER_FLIPDOT drivers.
 *
 * @section intro_sec Introduction
 *
 * This is documentation for a HANOVER_FLIPDOT library
 *
 * These displays use a 14-pin Lumberg MICA14 connector. It connects to 
 * two binary ripple counters (CD4024BC), one which keeps track of the 
 * current row, and one to keep track of the current column. Another input 
 * will select whether the dot at the intersection of these indices will 
 * be on (yellow), or off (black). The interface includes the following pins:
 * Column Advance Pin: Increments the binary counter for the column.
 * 
 * Row Advance Pin: Increments the binary counter for the row.
 * 
 * Reset Pin: Resets both binary counters.
 * 
 * Set Pin: Used to select which way the current will be pulsed, when
 * the Coil Pulse pin us directed to do so.
 * 
 * 18v Coil Pulse Pin: Used to provide drive power to change the dot state.
 *
 * @section dependencies Dependencies
 *
 * This library depends on <a
 * href="https://github.com/adafruit/Adafruit-GFX-Library"> Adafruit_GFX</a>
 * being present on your system. Please make sure you have installed the latest
 * version before using this library.
 *
 * @section author Author
 *
 * Written by Andrew Littlejohn (Caustic) for LMNC, with
 * contributions from the open source community. This is based on existing device drivers made available by Adafruit.
 *
 * @section license License
 *
 * BSD license, all text above, and the splash screen included below,
 * must be included in any redistribution.
 *
 */

#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
#include <pgmspace.h>
#else
#define pgm_read_byte(addr)                                                    \
  (*(const unsigned char *)(addr)) ///< PROGMEM workaround for non-AVR
#endif

#include "Adafruit_HANOVER_FLIPDOT.h"
#include <Adafruit_GFX.h>

// CONSTRUCTORS, DESTRUCTOR ------------------------------------------------

/*!
    @brief  Constructor for HANOVER_FLIPDOT displays.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  reset_pin
            Reset pin (using Arduino pin numbering), or -1 if not used.
    @param  row_adv_pin
            The pin used to advance the row.
    @param  col_adv_pin
            The pin used to advance the coloumn.
    @param  coil_pulse_pin
            Used to provide drive power to change the dot state.
    @param  set_pin
            Used to select which way the current will be pulsed, when 
            the Coil Pulse pin us directed to do so.
    @param  disp1_enable_pin
            The pin used to select display 1.
    @param  disp2_enable_pin
            The pin used to select display 2.
    @param  disp3_enable_pin
            The pin used to select display 3.
    @param  disp4_enable_pin
            The pin used to select display 4.
    @return Adafruit_HANOVER_FLIPDOT object.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!

    We us initialization lists to initialize data members of the class.
    The list of members to be initialized is indicated with a constructor as 
    a comma-separated list followed by a colon.
*/
Adafruit_HANOVER_FLIPDOT::Adafruit_HANOVER_FLIPDOT(uint8_t w, uint8_t h, int8_t reset_pin, int8_t row_adv_pin, int8_t col_adv_pin, int8_t coil_pulse_pin, int8_t set_pin, int8_t disp1_enable_pin, int8_t disp2_enable_pin, int8_t disp3_enable_pin, int8_t disp4_enable_pin): Adafruit_GFX(w, h), buffer(NULL), reset_pin(reset_pin), row_adv_pin(row_adv_pin), col_adv_pin(col_adv_pin), coil_pulse_pin(coil_pulse_pin), set_pin(set_pin), disp1_enable_pin(disp1_enable_pin), disp2_enable_pin(disp2_enable_pin), disp3_enable_pin(disp3_enable_pin), disp4_enable_pin(disp4_enable_pin) 

/*!
    @brief  Destructor for Adafruit_HANOVER_FLIPDOT object.
*/
Adafruit_HANOVER_FLIPDOT::~Adafruit_HANOVER_FLIPDOT(void) {
  if (buffer) {
    free(buffer);
    buffer = NULL;
  }
}

// ALLOCATE & INIT DISPLAY -------------------------------------------------

/*!
    @brief  Allocate RAM for image buffer, initialize peripherals and pins.
    @param  addr
            I2C address of corresponding HANOVER_FLIPDOT display (or pass 0 to use
            default of 0x3C for 128x32 display, 0x3D for all others).
            SPI displays (hardware or software) do not use addresses, but
            this argument is still required (pass 0 or any value really,
            it will simply be ignored). Default if unspecified is 0.
    @param  reset
            If true, and if the reset pin passed to the constructor is
            valid, a hard reset will be performed before initializing the
            display. If using multiple HANOVER_FLIPDOT displays on the same bus, and
            if they all share the same reset pin, you should only pass true
            on the first display being initialized, false on all others,
            else the already-initialized displays would be reset. Default if
            unspecified is true.
    @return true on successful allocation/init, false otherwise.
            Well-behaved code should check the return value before
            proceeding.
    @note   MUST call this function before any drawing or updates!
*/
bool Adafruit_HANOVER_FLIPDOT::begin(bool reset, uint8_t display_idx) {
  // Note: we have put in place the means to specify a display_idx, but there
  // may not be enough mem to actually buffer all of them separately.
  // Additionally, we dont have safe pin disabling, so just set display 1 for now.

  if ((!buffer) && !(buffer = (uint8_t *)malloc(WIDTH * ((HEIGHT + 7) / 8))))
    return false;

  clearDisplay();

  // Setup pin directions
  
  pinMode(row_adv_pin, OUTPUT);        ///< The pin used to advance the row. Set during construction.
  pinMode(col_adv_pin, OUTPUT);        ///< The pin used to advance the col. Set during construction.
  pinMode(coil_pulse_pin, OUTPUT);     ///< Used to provide drive power to change the dot state. Set during construction.
  pinMode(set_pin, OUTPUT);            ///< Used to select which way the current will be pulsed. Set during construction.
  pinMode(disp1_enable_pin, OUTPUT);   ///< The pin used to select display 1. Set during construction.
  pinMode(disp2_enable_pin, OUTPUT);   ///< The pin used to select display 2. Set during construction.
  pinMode(disp3_enable_pin, OUTPUT);   ///< The pin used to select display 3. Set during construction.
  pinMode(disp4_enable_pin, OUTPUT);   ///< The pin used to select display 4. Set during construction.

  // Set the enable pin high for the display.
  switch(display_idx){
    case 1 : 
      digitalWrite(disp1_enable_pin, HIGH);
    case 2 : 
      digitalWrite(disp2_enable_pin, HIGH); 
    case 3 : 
      digitalWrite(disp3_enable_pin, HIGH); 
    case 4 : 
      digitalWrite(disp4_enable_pin, HIGH);  
  }

  // Reset HANOVER_FLIPDOT if requested and reset pin specified in constructor
  if (reset && (reset_pin >= 0)) {
    pinMode(reset_pin, OUTPUT);        ///< The pin used to reset both row and col binary counters. Set during construction.
    digitalWrite(reset_pin, HIGH);
    delay(1);                          // VDD goes high at start, pause for 1 ms
    digitalWrite(reset_pin, LOW);      // Bring reset low
    delay(10);                         // Wait 10 ms
    digitalWrite(reset_pin, HIGH);     // Bring out of reset
    uint8_t col_idx = 0; // reset column index
    uint8_t row_idx = 0; // reset row index
  }

  // Init sequence
  // insert some drawPixel stuff to show things happend.

  return true; // Success
}

// DRAWING FUNCTIONS -------------------------------------------------------

/*!
    @brief  Set/clear/invert a single pixel. This is also invoked by the
            Adafruit_GFX library in generating many higher-level graphics
            primitives.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @param  color
            Pixel color, one of: HANOVER_FLIPDOT_BLACK, HANOVER_FLIPDOT_YELLOW or
            HANOVER_FLIPDOT_INVERSE.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void Adafruit_HANOVER_FLIPDOT::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x >= 0) && (x < width()) && (y >= 0) && (y < height())) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (getRotation()) {
    case 1:
      HANOVER_FLIPDOT_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      break;
    case 3:
      HANOVER_FLIPDOT_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    switch (color) {
    case HANOVER_FLIPDOT_YELLOW:
      buffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
      break;
    case HANOVER_FLIPDOT_BLACK:
      buffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
      break;
    case HANOVER_FLIPDOT_INVERSE:
      buffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
      break;
    }
  }
}

/*!
    @brief  Clear contents of display buffer (set all pixels to off).
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(), or with other graphics
            commands as needed by one's own application.
*/
void Adafruit_HANOVER_FLIPDOT::clearDisplay(void) {
  memset(buffer, 0, WIDTH * ((HEIGHT + 7) / 8));
}

/*!
    @brief  Return color of a single pixel in display buffer.
    @param  x
            Column of display -- 0 at left to (screen width - 1) at right.
    @param  y
            Row of display -- 0 at top to (screen height -1) at bottom.
    @return true if pixel is set (usually HANOVER_FLIPDOT_YELLOW, unless display invert
   mode is enabled), false if clear (HANOVER_FLIPDOT_BLACK).
    @note   Reads from buffer contents; may not reflect current contents of
            screen if display() has not been called.
*/
bool Adafruit_HANOVER_FLIPDOT::getPixel(int16_t x, int16_t y) {
  if ((x >= 0) && (x < width()) && (y >= 0) && (y < height())) {
    // Pixel is in-bounds. Rotate coordinates if needed.
    switch (getRotation()) {
    case 1:
      HANOVER_FLIPDOT_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      break;
    case 3:
      HANOVER_FLIPDOT_swap(x, y);
      y = HEIGHT - y - 1;
      break;
    }
    return (buffer[x + (y / 8) * WIDTH] & (1 << (y & 7)));
  }
  return false; // Pixel out of bounds
}

/*!
    @brief  Get base address of display buffer for direct reading or writing.
    @return Pointer to an unsigned 8-bit array, column-major, columns padded
            to full byte boundary if needed.
*/
uint8_t *Adafruit_HANOVER_FLIPDOT::getBuffer(void) { return buffer; }

// REFRESH DISPLAY ---------------------------------------------------------
/*
************************************
************************************
************************************
************************************
CHANGE THIS!!!!!!
************************************
************************************
************************************
************************************
*/
/*!
    @brief  Push data currently in RAM to HANOVER_FLIPDOT display.
    @return None (void).
    @note   Drawing operations are not visible until this function is
            called. Call after each graphics command, or after a whole set
            of graphics commands, as best needed by one's own application.
*/
void Adafruit_HANOVER_FLIPDOT::display(void) {
  TRANSACTION_START
  static const uint8_t PROGMEM dlist1[] = {
      HANOVER_FLIPDOT_PAGEADDR,
      0,                      // Page start address
      0xFF,                   // Page end (not really, but works here)
      HANOVER_FLIPDOT_COLUMNADDR, 0}; // Column start address
  HANOVER_FLIPDOT_commandList(dlist1, sizeof(dlist1));
  HANOVER_FLIPDOT_command1(WIDTH - 1); // Column end address

  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = buffer;
  if (wire) { // I2C
    wire->beginTransmission(i2caddr);
    WIRE_WRITE((uint8_t)0x40);
    uint16_t bytesOut = 1;
    while (count--) {
      if (bytesOut >= WIRE_MAX) {
        wire->endTransmission();
        wire->beginTransmission(i2caddr);
        WIRE_WRITE((uint8_t)0x40);
        bytesOut = 1;
      }
      WIRE_WRITE(*ptr++);
      bytesOut++;
    }
    wire->endTransmission();
  } else { // SPI
    HANOVER_FLIPDOT_MODE_DATA
    while (count--)
      SPIwrite(*ptr++);
  }
  TRANSACTION_END
