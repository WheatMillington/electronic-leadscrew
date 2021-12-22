// Clough42 Electronic Leadscrew
// https://github.com/clough42/electronic-leadscrew
//
// MIT License
//
// Copyright (c) 2019 James Clough
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef __CONTROL_PANEL_2_H
#define __CONTROL_PANEL_2_H

#include "F28x_Project.h"
#include "SPIBus_2.h"

#define LED_xTPI 1
#define LED_xINCH (1<<1)
#define LED_xMM (1<<2)
#define LED_xTHREAD (1<<3)
#define LED_xFEED (1<<4)
#define LED_xREVERSE (1<<5)
#define LED_xFORWARD (1<<6)
#define LED_xPOWER (1<<7)

struct LED_BITS
{
    Uint16 xTPI:1;
    Uint16 xINCH:1;
    Uint16 xMM:1;
    Uint16 xTHREAD:1;
    Uint16 xFEED:1;
    Uint16 xREVERSE:1;
    Uint16 xFORWARD:1;
    Uint16 xPOWER:1;
};

typedef union LED_REG
{
    Uint16 all;
    struct LED_BITS bit;
} LED_REG;

struct KEY_BITS
{
    Uint16 xUP:1;
    Uint16 xreserved1:1;
    Uint16 xDOWN:1;
    Uint16 xIN_MM:1;
    Uint16 xFEED_THREAD:1;
    Uint16 xFWD_REV:1;
    Uint16 xSET:1;
    Uint16 xPOWER:1;
};

typedef union KEY_REG
{
    Uint16 all;
    struct KEY_BITS bit;
} KEY_REG;


class ControlPanel_2
{
private:
    // Common SPI Bus
    SPIBus_2 *spiBus;

    // Current RPM value; 4 decimal digits
    Uint16 rpm;

    // Current displayed setting value, 4 digits
    const Uint16 *value;

    // Current LED states
    LED_REG leds;

    // current key states
    KEY_REG keys;

    // number of times current key state has been seen
    KEY_REG stableKeys;
    Uint16 stableCount;

    // current override message, or NULL if none
    const Uint16 *message;

    // brightness, levels 1-8, 0=off
    Uint16 brightness;

    // Derived state, calculated internally
    Uint16 sevenSegmentData[8];

    // dummy register, for SPI
    Uint16 dummy;

    void decomposeValue(void);
    KEY_REG readKeys(void);
    Uint16 lcd_char(Uint16 x);
    void sendByte(Uint16 data);
    Uint16 receiveByte(void);
    void sendData(void);
    Uint16 reverse_byte(Uint16 x);
    void initSpi();
    void configureSpiBus(void);
    bool isValidKeyState(KEY_REG);
    bool isStable(KEY_REG);

public:
    ControlPanel_2(SPIBus_2 *spiBus);

    // initialize the hardware for operation
    void initHardware(void);

    // poll the keys and return a mask
    KEY_REG getKeys(void);

    // set the value to display
    void setValue(const Uint16 *value);

    // set the LED states
    void setLEDs(LED_REG leds);

    // set a message that overrides the display, 8 characters required
    void setMessage(const Uint16 *message);

    // set a brightness value, 0 (off) to 8 (max)
    void setBrightness(Uint16 brightness);

    // refresh the hardware display
    void refresh();
};


inline void ControlPanel_2 :: setValue(const Uint16 *value)
{
    this->value = value;
}

inline void ControlPanel_2 :: setLEDs(LED_REG leds)
{
    this->leds = leds;
}


#endif // __CONTROL_PANEL_2_H
