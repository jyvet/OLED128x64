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

#ifndef __OLED128X64_H__
#define __OLED128X64_H__

#include <Arduino.h>
#include <functional>

#define OLED_WIDTH                   128
#define OLED_HEIGHT                  64


/* ============================== Register names ============================ */

#define OLED_ADDRESS                 0x3C
#define OLED_DATA_MODE               0x40
#define OLED_CMD_MODE                0x80
#define OLED_NORMALDISPLAY           0xA6
#define OLED_OFF                     0xAE
#define OLED_ON                      0xAF
#define OLED_SETDISPLAYCLOCKDIV      0xD5
#define OLED_SETMULTIPLEX            0xA8
#define OLED_SETDISPLAYOFFSET        0xD3
#define OLED_STARTLINE               0x40
#define OLED_CHARGEPUMP              0x8D
#define OLED_SETMEMORYMODE           0x20
#define OLED_SEGREMAP                0xA0
#define OLED_COMSCANDEC              0xC8
#define OLED_COMSCANINC              0xC0
#define OLED_SETCOMPINS              0xDA
#define OLED_SETCONTRAST             0x81
#define OLED_SETPRECHARGE            0xD9
#define OLED_SETVCOMDETECT           0xDB
#define OLED_DISPLAYALLONRESUME      0xA4
#define OLED_SCROLLON                0x2F
#define OLED_SCROLLOFF               0x2E
#define OLED_INVERTDISPLAY           0xA7
#define OLED_LOWCOLUMN               0x00
#define OLED_HIGHCOLUMN              0x10


class OLED128x64
{
    public:
        OLED128x64(const byte address = OLED_ADDRESS);

        void      setAddress(const byte address);
        byte      getAddress();
        void      setInvertedDisplay(const bool val);
        void      setPixel(const int row, const int col, const bool val);
        void      setCharCursor(const int X, const int col);
        void      setCursor(const int X, const int Y);

        void      init();
        void      powerOn();
        void      powerOff();

        void      clear();
        void      clearCharRow(const int X);
        void      clearChar(const int X, const int Y);

        void      drawStr(const char *string, int X, int Y);
        void      drawStr(const char *string, int X, int Y,
                          const char font[256][8]);
        void      drawImage(const char *data);
        void      drawHLine(const int Y);
        void      drawVLine(const int X);
        void      drawProgressBar(const int Y, const int percent);

    private:
        byte      _address;
        void      _write(const byte data, const byte mode);
        void      _clearBuffer();
        void      _clearScreen();
        void      _clearCharBuffer(const int X, const int Y);
        void      _clearCharScreen(const int X, const int Y);
        byte      _buffer[OLED_WIDTH][OLED_HEIGHT/8];
};

extern OLED128x64 OLED;
extern const char defaultFont[][8];
#endif
