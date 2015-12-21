/*******************************************************************************
* Copyright (C) 2015, Jean-Yves VET, contact [at] jean-yves [dot] vet          *
*                                                                              *
* This software is licensed as described in the file LICENCE, which you should *
* have received as part of this distribution. You may opt to use, copy,        *
* modify, merge, publish, distribute and/or sell copies of the Software, and   *
* permit persons to whom the Software is furnished to do so, under the terms   *
* of the LICENCE file.                                                         *
*                                                                              *
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY    *
* KIND, either express or implied.                                             *
*******************************************************************************/

/**
 * @file OLED128x64.cpp
 * @brief Interface for 128x64 OLED screen (I2C) with SSD1306 driver.
 * @author Jean-Yves VET
 */

#include <Wire.h>
#include "OLED128x64.h"


/* ========================= Constructor / Destructor ======================= */

/**
 * Create an OLED128x64 object.
 *
 * @param   address[in]    I2C address of the device
 */
OLED128x64::OLED128x64(const byte address) {
    setAddress(address);
}


/* ============================ Private Functions =========================== */

/**
 * Write byte via I2C.
 *
 * @param   byte[in]      Byte to write
 * @param   mode[in]      Mode command (OLED_CMD_MODE) or data (OLED_DATA_MODE)
 */
void OLED128x64::_write(const byte data, const byte mode) {
    Wire.beginTransmission(_address);
    Wire.write(mode);
    Wire.write(data);
    Wire.endTransmission();
}


/**
 * Clear screen buffer.
 */
void OLED128x64::_clearBuffer() {
    int i,j;
    for(i=0; i<OLED_WIDTH; i++) {
        for(j=0; j<OLED_HEIGHT/8; j++) {
            _buffer[i][j] = 0;
        }
    }
}


/**
 * Clear screen.
 */
void OLED128x64::_clearScreen() {
    int i,j;

    for(j=0; j<OLED_HEIGHT/8; j++) {
      setCharCursor(j,0);
      for(i=0; i<OLED_WIDTH; i++) {
          _write(0, OLED_DATA_MODE);
      }
    }

    setCharCursor(0,0);
}


/**
 * Clear character in buffer.
 *
 * @param  X      Character row [0 to 7]
 * @param  Y      Character column [0 to 15]
 */
void OLED128x64::_clearCharBuffer(const int X, const int Y) {
    int i;

    for(i=0; i<8; i++) {
        _buffer[Y*8+i][X] = 0;
    }
}


/**
 * Clear character on screen.
 *
 * @param  X      Character row [0 to 7]
 * @param  Y      Character column [0 to 15]
 */
void OLED128x64::_clearCharScreen(const int X, const int Y) {
    int i;

    setCharCursor(X, Y);

    for(i=0; i<8; i++) {
        _write(0x00, OLED_DATA_MODE);
    }
}


/* ============================ Getters / Setters =========================== */

/**
 * Get address byte.
 *
 * @return  byte containing the I2C address of the device
 */
byte OLED128x64::getAddress() {
    return _address;
}


/**
 * Set address byte.
 *
 * @param  address  Byte containing the I2C address of the device
 */
void OLED128x64::setAddress(const byte address) {
    _address = address;
}


/**
 * Set/unset inverted display mode.
 *
 * @param  val  If true inverted mode is enabled
 */
void OLED128x64::setInvertedDisplay(const bool val) {
    if (val) {
        _write(OLED_INVERTDISPLAY, OLED_CMD_MODE);
    } else {
        _write(OLED_NORMALDISPLAY, OLED_CMD_MODE);
    }
}


/**
 * Set/unset a pixel.
 *
 * @param  row     Pixel row [0 to 63]
 * @param  col     Pixel column [0 to 127]
 * @param  val     If true, show pixel
 */
void OLED128x64::setPixel(const int row, const int col, const bool val) {
    _buffer[col][row/8] |= (val << (row%8));

    _write(0xB0 + row/8,                              OLED_CMD_MODE);
    _write(OLED_LOWCOLUMN + (col & 0x0F),             OLED_CMD_MODE);
    _write(OLED_HIGHCOLUMN + ((col>>4) & 0x0F),       OLED_CMD_MODE);
    _write(_buffer[col][row/8],                       OLED_DATA_MODE);
}


/**
 * Set cursor position for 8x8 pixels char.
 *
 * @param  X      Character row [0 to 7]
 * @param  Y      Character column [0 to 15]
 */
void OLED128x64::setCharCursor(const int X, const int Y) {
  _write(0xB0 + X,                                    OLED_CMD_MODE);
  _write(OLED_LOWCOLUMN + (8*Y & 0x0F),               OLED_CMD_MODE);
  _write(OLED_HIGHCOLUMN + ((8*Y>>4) & 0x0F),         OLED_CMD_MODE);
}


/**
 * Set cursor position.
 *
 * @param  X       Row [0 to 7]
 * @param  col     Pixel column [0 to 127]
 */
void OLED128x64::setCursor(const int X, const int col) {
  _write(0xB0 + X,                                    OLED_CMD_MODE);
  _write(OLED_LOWCOLUMN + (col & 0x0F),               OLED_CMD_MODE);
  _write(OLED_HIGHCOLUMN + ((col>>4) & 0x0F),         OLED_CMD_MODE);
}


/* ============================ Public Functions ============================ */

/**
 * Initialize sceen with default settings
 */
void OLED128x64::init() {
    _write(OLED_OFF,                             OLED_CMD_MODE);
    _write(OLED_NORMALDISPLAY,                   OLED_CMD_MODE);

    _write(OLED_SETDISPLAYCLOCKDIV,              OLED_CMD_MODE);
    _write(0x80,                                 OLED_CMD_MODE);

    _write(OLED_SETMULTIPLEX,                    OLED_CMD_MODE);
    _write(0x3F,                                 OLED_CMD_MODE);

    _write(OLED_SETDISPLAYOFFSET,                OLED_CMD_MODE);
    _write(0x00,                                 OLED_CMD_MODE);

    _write(OLED_STARTLINE | 0x00,                OLED_CMD_MODE);

    _write(OLED_CHARGEPUMP,                      OLED_CMD_MODE);
    _write(0x14,                                 OLED_CMD_MODE);

    _write(OLED_SETMEMORYMODE,                   OLED_CMD_MODE);
    _write(0x00,                                 OLED_CMD_MODE);

    _write(OLED_SEGREMAP,                        OLED_CMD_MODE);

    _write(OLED_COMSCANDEC,                      OLED_CMD_MODE);

    _write(OLED_SETCOMPINS,                      OLED_CMD_MODE);
    _write(0x12,                                 OLED_CMD_MODE);

    _write(OLED_SETCONTRAST,                     OLED_CMD_MODE);
    _write(0xCF,                                 OLED_CMD_MODE);

    _write(OLED_SETPRECHARGE,                    OLED_CMD_MODE);
    _write(0xF1,                                 OLED_CMD_MODE);

    _write(OLED_SETVCOMDETECT,                   OLED_CMD_MODE);
    _write(0x40,                                 OLED_CMD_MODE);

    _write(OLED_DISPLAYALLONRESUME,              OLED_CMD_MODE);

    _write(OLED_SEGREMAP,                        OLED_CMD_MODE);
    _write(0xa1,                                 OLED_CMD_MODE);

    _write(OLED_SCROLLOFF,                       OLED_CMD_MODE);

    clear();
    powerOn();
}


/**
 * Clear all displayed data and buffer.
 */
void OLED128x64::clear() {
    _clearScreen();
    _clearBuffer();
    setCharCursor(0,0);
}


/**
 * Clear character on screen and in buffer.
 *
 * @param  X      Character row [0 to 7]
 * @param  Y      Character column [0 to 15]
 */
void OLED128x64::clearChar(const int X, const int Y) {
    _clearCharBuffer(X, Y);
    _clearCharScreen(X, Y);
    setCharCursor(0,0);
}


/**
 * Clear character row on screen and in buffer.
 *
 * @param  X      Character row [0 to 7]
 */
void OLED128x64::clearCharRow(const int X) {
    int j;

    for (j=0; j<OLED_WIDTH/8; j++) {
        _clearCharBuffer(X, j);
        _clearCharScreen(X, j);
    }
    setCharCursor(0,0);
}


/**
 * Power off the screen to save power.
 */
void OLED128x64::powerOn() {
    _write(OLED_ON, OLED_CMD_MODE);
}


/**
 * Power on the screen.
 */
void OLED128x64::powerOff() {
    _write(OLED_OFF, OLED_CMD_MODE);
}


/**
 * Draw a progress bar.
 *
 * @param  X              Character row [0 to 7]
 * @param  percent        Percentage
 */
void OLED128x64::drawProgressBar(const int X, const int percent) {
    int i;

    /* Clean row */
    if(percent == 0) {
        clearCharRow(X);
    }

    /* Draw Layout */
    setCursor(X, 12);
    _write(0x7E, OLED_DATA_MODE);
    setCursor(X, 113);
    _write(0x7E, OLED_DATA_MODE);

    /* Draw Progress Bar */
    setCursor(X, 13);
    for(i=0;i<100;i++) {
        if (percent >= i) {
            _write(0x7E, OLED_DATA_MODE);
        } else {
            _write(0x42, OLED_DATA_MODE);
        }
    }
}


/**
 * Draw string of char using default font.
 *
 * @param  string         String to draw
 * @param  X              Start row [0 to 7]
 * @param  Y              Start column [0 to 15]
 */
void OLED128x64::drawStr(const char *string, const int X, const int Y) {
    drawStr(string, X, Y, defaultFont);
}


/**
 * Draw string of char using a provided font.
 *
 * @param  string         String to draw
 * @param  X              Start row [0 to 7]
 * @param  Y              Start column [0 to 15]
 * @param  font           Font to use
 */
void OLED128x64::drawStr(const char *string, const int X, const int Y,
                                                      const char font[256][8]) {
    setCharCursor(X,Y);
    char i=0;

    while(*string) {
        for(i=0;i<8;i++) {
            _write(pgm_read_byte(font[(unsigned char)*string]+i),
                                                                OLED_DATA_MODE);
        }
        *string++;
    }
}


/**
 * Draw an image.
 *
 * @param  data         Image to draw
 */
void OLED128x64::drawImage(const char *data){
    int i;

    clear();
    setCharCursor(0,0);
    for(int i=0; i<OLED_WIDTH*OLED_HEIGHT/8;i++) {
        _write(pgm_read_byte(data + i), OLED_DATA_MODE);
    }
}


/**
 * Draw a horizontal line.
 *
 * @param  row         Row where the line has to be drawn
 */
void OLED128x64::drawHLine(const int row){
    char i;
    setCharCursor(row/8,0);
    unsigned char value = 0;
    value |= (1 << (row%8));
    for(i=0;i<128;i++) {
        _write(value, OLED_DATA_MODE);
    }
    setCharCursor(0,0);
}


/**
 * Draw a vertical line.
 *
 * @param  col         Column where the line has to be drawn
 */
void OLED128x64::drawVLine(const int col){
    char i,k;
    for(k=0; k<8; k++) {
      setCursor(k,col);
      _write(0xFF, OLED_DATA_MODE);
    }
    setCharCursor(0,0);
}


OLED128x64 OLED;


/* Default font: Quadrang */
const char defaultFont[][8] PROGMEM = {
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00},
{0x00, 0x0c, 0x1e, 0x1e, 0x1e, 0x0c, 0x00, 0x00},
{0x00, 0x16, 0x16, 0x16, 0x16, 0x1e, 0x1e, 0x00},
{0x06, 0x16, 0x16, 0x0e, 0x1c, 0x1e, 0x1e, 0x1c},
{0x00, 0x1a, 0x1e, 0x1e, 0x1e, 0x16, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x3c, 0x3e, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x3e, 0x3c, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x04, 0x06, 0x06, 0x04, 0x00, 0x00},
{0x00, 0x08, 0x08, 0x1c, 0x1c, 0x08, 0x00, 0x00},
{0x30, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00},
{0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x10, 0x1c, 0x06, 0x00, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x12, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x16, 0x16, 0x16, 0x00, 0x00},
{0x00, 0x12, 0x16, 0x16, 0x1e, 0x1a, 0x00, 0x00},
{0x00, 0x0e, 0x0e, 0x08, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x16, 0x16, 0x16, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x16, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x02, 0x12, 0x1a, 0x0e, 0x02, 0x00, 0x00},
{0x00, 0x1a, 0x1e, 0x16, 0x1e, 0x1a, 0x00, 0x00},
{0x00, 0x16, 0x16, 0x16, 0x1e, 0x1e, 0x00, 0x00},
{0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x34, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x08, 0x08, 0x08, 0x14, 0x14, 0x00, 0x00},
{0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},
{0x00, 0x14, 0x14, 0x18, 0x08, 0x08, 0x00, 0x00},
{0x02, 0x16, 0x16, 0x06, 0x06, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x0a, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x16, 0x1e, 0x1a, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x12, 0x12, 0x12, 0x12, 0x00},
{0x00, 0x1e, 0x1e, 0x12, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x16, 0x16, 0x16, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x06, 0x06, 0x02, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x16, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x04, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00},
{0x00, 0x10, 0x10, 0x10, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x0c, 0x12, 0x12, 0x00, 0x00},
{0x00, 0x00, 0x1e, 0x1e, 0x10, 0x10, 0x00, 0x00},
{0x1e, 0x1e, 0x0c, 0x18, 0x0c, 0x1e, 0x1e, 0x00},
{0x00, 0x1e, 0x1e, 0x0c, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x12, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x0a, 0x0e, 0x0e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x12, 0x3e, 0x3e, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x06, 0x1e, 0x1a, 0x00, 0x00},
{0x00, 0x16, 0x16, 0x16, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x02, 0x02, 0x1e, 0x1e, 0x02, 0x02, 0x00},
{0x00, 0x1e, 0x1e, 0x10, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x02, 0x06, 0x18, 0x18, 0x0e, 0x02, 0x00},
{0x1e, 0x1e, 0x10, 0x1e, 0x1e, 0x10, 0x1e, 0x1e},
{0x00, 0x10, 0x1a, 0x0e, 0x0e, 0x1a, 0x10, 0x00},
{0x00, 0x02, 0x06, 0x1c, 0x1c, 0x06, 0x02, 0x00},
{0x00, 0x02, 0x12, 0x1a, 0x16, 0x12, 0x12, 0x00},
{0x00, 0x00, 0x1e, 0x1e, 0x12, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x02, 0x06, 0x1c, 0x10, 0x00, 0x00},
{0x00, 0x00, 0x12, 0x12, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x08, 0x0c, 0x06, 0x0e, 0x08, 0x00, 0x00},
{0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x14, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x14, 0x14, 0x14, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x14, 0x1e, 0x1e, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x00, 0x1e, 0x1e, 0x06, 0x02, 0x00, 0x00},
{0x00, 0x3c, 0x3c, 0x34, 0x3c, 0x3c, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x04, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x3e, 0x3e, 0x00, 0x00, 0x00},
{0x00, 0x1e, 0x1e, 0x08, 0x14, 0x10, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00},
{0x1c, 0x1c, 0x04, 0x1c, 0x1c, 0x04, 0x1c, 0x1c},
{0x00, 0x1c, 0x1c, 0x04, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x14, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x3c, 0x3c, 0x14, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x14, 0x3c, 0x3c, 0x00, 0x00},
{0x00, 0x00, 0x1c, 0x1c, 0x04, 0x04, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x00, 0x1e, 0x1e, 0x14, 0x00, 0x00, 0x00},
{0x00, 0x1c, 0x1c, 0x10, 0x1c, 0x1c, 0x00, 0x00},
{0x00, 0x04, 0x1c, 0x18, 0x1c, 0x0c, 0x00, 0x00},
{0x1c, 0x1c, 0x10, 0x1c, 0x1c, 0x1c, 0x1c, 0x00},
{0x00, 0x00, 0x00, 0x14, 0x08, 0x1c, 0x14, 0x00},
{0x00, 0x3c, 0x3c, 0x30, 0x3c, 0x3c, 0x00, 0x00},
{0x00, 0x04, 0x14, 0x1c, 0x1c, 0x14, 0x00, 0x00},
{0x00, 0x00, 0x1c, 0x3e, 0x22, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x3e, 0x3c, 0x08, 0x00, 0x00, 0x00},
{0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00}
};