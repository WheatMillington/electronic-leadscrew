/*
 * SPIBus.cpp
 *
 *  Created on: Aug 15, 2019
 *      Author: clough
 */

#include "SPIBus.h"
#include "F28x_Project.h"

// wait for the current serial shift operation to complete
#define WAIT_FOR_SERIAL while(SpibRegs.SPISTS.bit.INT_FLAG !=1) {}
#define WAIT_FOR_SERIAL_2 while(SpiaRegs.SPISTS.bit.INT_FLAG !=1) {}


SPIBus :: SPIBus( void )
{
    mask = 0xffff;
}

void SPIBus :: initHardware(void)
{
    // Set up slow speed clock
    EALLOW;
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0b100; // LPSCLK = SYSCLK/8 = 12.5MHz
    EDIS;

    // Set up SPI
    setEightBits();
    setThreeWire();

    // Set up SPI A (Display 2)
    SpiaRegs.SPICCR.bit.SPISWRESET = 0; // Enter RESET state

    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1; // data latched on rising edge
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0; // normal clocking scheme
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; // master
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 127; // SPI bit rate = LPSCLK/128 ~ 98Kbps

    SpiaRegs.SPICCR.bit.SPISWRESET = 1; // clear reset state; ready to transmit

    // Set up SPI B (Display 1)
    SpibRegs.SPICCR.bit.SPISWRESET = 0; // Enter RESET state

    SpibRegs.SPICCR.bit.CLKPOLARITY = 1; // data latched on rising edge
    SpibRegs.SPICTL.bit.CLK_PHASE = 0; // normal clocking scheme
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1; // master
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = 127; // SPI bit rate = LPSCLK/128 ~ 98Kbps

    SpibRegs.SPICCR.bit.SPISWRESET = 1; // clear reset state; ready to transmit

    EALLOW;

    // Set up muxing for SPIA pins (Display 2)
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0x1;      // select SPIA_SIMO
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0x0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0x1;      // select SPIA_SOMI
    GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 0x0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0x1;      // select SPIA_CLK
    GpioCtrlRegs.GPBGMUX2.bit.GPIO56 = 0x0;

    // Set up muxing for SPIB pins (Display 1)
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0x2;      // select SPIB_SIMO
    GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 0x1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0x3;      // select SPIB_SOMI
    GpioCtrlRegs.GPAGMUX2.bit.GPIO31 = 0x0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0x3;      // select SPIB_CLK
    GpioCtrlRegs.GPBGMUX1.bit.GPIO32 = 0x0;

    EDIS;
}

void SPIBus :: setThreeWire( void )
{
    SpiaRegs.SPIPRI.bit.TRIWIRE = 1; // 3-wire mode
    SpibRegs.SPIPRI.bit.TRIWIRE = 1; // 3-wire mode
}

void SPIBus :: setFourWire( void )
{
    SpiaRegs.SPIPRI.bit.TRIWIRE = 0; // Normal (4-wire) mode
    SpibRegs.SPIPRI.bit.TRIWIRE = 0; // Normal (4-wire) mode
}

void SPIBus :: setEightBits( void )
{
    SpiaRegs.SPICCR.bit.SPICHAR = 0x7; // 8 bits
    SpibRegs.SPICCR.bit.SPICHAR = 0x7; // 8 bits
    mask = 0x00ff;                     // set the mask to 8 bits
}

void SPIBus :: setSixteenBits( void )
{
    SpiaRegs.SPICCR.bit.SPICHAR = 0xF; // 16 bits
    SpibRegs.SPICCR.bit.SPICHAR = 0xF; // 16 bits
    mask = 0xffff;                     // set the mask to 16 bits
}

void SPIBus :: sendWord(Uint16 data)
{
    SpibRegs.SPICTL.bit.TALK = 1;
    SpibRegs.SPITXBUF = data;
    WAIT_FOR_SERIAL;
    dummy = SpibRegs.SPIRXBUF;
}

void SPIBus :: sendWord_2(Uint16 data) // Send word to second display
{
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPITXBUF = data;
    WAIT_FOR_SERIAL_2;
    dummy_2 = SpiaRegs.SPIRXBUF;
}

Uint16 SPIBus :: receiveWord(void) {
    SpibRegs.SPICTL.bit.TALK = 0;
    SpibRegs.SPITXBUF = dummy;
    WAIT_FOR_SERIAL;
    return SpibRegs.SPIRXBUF & mask; // mask off if we're in 8-bit mode
}

Uint16 SPIBus :: receiveWord_2(void) { // Receive word from second display
    SpiaRegs.SPICTL.bit.TALK = 0;
    SpiaRegs.SPITXBUF = dummy_2;
    WAIT_FOR_SERIAL_2; //change back to wait_for_serial_2
    return SpiaRegs.SPIRXBUF & mask; // mask off if we're in 8-bit mode
}




