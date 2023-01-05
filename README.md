# Adafruit_HANOVER_FLIPDOT

This is an Arduino library for HANOVER_FLIPDOT drivers.

## Introduction
This is documentation for a HANOVER_FLIPDOT library

These displays use a 14-pin Lumberg MICA14 connector. It connects to 
two binary ripple counters (CD4024BC), one which keeps track of the 
current row, and one to keep track of the current column. Another input 
will select whether the dot at the intersection of these indices will 
be on (yellow), or off (black). The interface includes the following pins:

Column Advance Pin: Increments the binary counter for the column.
Row Advance Pin: Increments the binary counter for the row.
Reset Pin: Resets both binary counters.
Set Pin: Used to select which way the current will be pulsed, when the Coil Pulse pin us directed to do so.
18v Coil Pulse Pin: Used to provide drive power to change the dot state.

## Dependencies
You will also have to install the **Adafruit GFX library** which provides graphics primitves such as lines, circles, text, etc. This also can be found in the Arduino Library Manager, or you can get the source from https://github.com/adafruit/Adafruit-GFX-Library

Preferred installation method is to use the Arduino IDE Library Manager. To download the source from Github instead, click "Clone or download" above, then "Download ZIP." After uncompressing, rename the resulting folder Adafruit_HANOVER_FLIPDOT. Check that the Adafruit_HANOVER_FLIPDOT folder contains Adafruit_HANOVER_FLIPDOT.cpp and Adafruit_HANOVER_FLIPDOT.h.

## Author
Written by Andrew Littlejohn (Caustic) for LMNC, with contributions from the open source community. This is based on existing device drivers made available by Adafruit.

## License
BSD license, all text above, must be included in any redistribution.