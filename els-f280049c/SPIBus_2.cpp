/*
 * SPIBus_2.cpp
 *
 *  Created on: Aug 15, 2019
 *      Author: clough
 */

#include "SPIBus_2.h"
#include "F28x_Project.h"

// wait for the current serial shift operation to complete
#define WAIT_FOR_SERIAL while(SpiaRegs.SPISTS.bit.INT_FLAG !=1) {}


SPIBus_2 :: SPIBus_2( void )
{
    mask = 0xffff;
}

void SPIBus_2 :: initHardware(void)
{
    // Set up slow speed clock
    EALLOW;
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0b100; // LPSCLK = SYSCLK/8 = 12.5MHz
    EDIS;

    // Set up SPI A
    SpiaRegs.SPICCR.bit.SPISWRESET = 0; // Enter RESET state
    setEightBits();
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1; // data latched on rising edge
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0; // normal clocking scheme
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; // master
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 127; // SPI bit rate = LPSCLK/128 ~ 98Kbps
    setThreeWire();
    SpiaRegs.SPICCR.bit.SPISWRESET = 1; // clear reset state; ready to transmit

    EALLOW;

    // Set up muxing for SPIA pins
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0x2;      // select SPIB_SIMO
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0x1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0x3;      // select SPIB_SOMI
    GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 0x0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0x3;      // select SPIB_CLK
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0x0;

    EDIS;
}

void SPIBus_2 :: setThreeWire( void )
{
    SpiaRegs.SPIPRI.bit.TRIWIRE = 1; // 3-wire mode
}

void SPIBus_2 :: setFourWire( void )
{
    SpiaRegs.SPIPRI.bit.TRIWIRE = 0; // Normal (4-wire) mode
}

void SPIBus_2 :: setEightBits( void )
{
    SpiaRegs.SPICCR.bit.SPICHAR = 0x7; // 8 bits
    mask = 0x00ff;                     // set the mask to 8 bits
}

void SPIBus_2 :: setSixteenBits( void )
{
    SpiaRegs.SPICCR.bit.SPICHAR = 0xF; // 16 bits
    mask = 0xffff;                     // set the mask to 16 bits
}

void SPIBus_2 :: sendWord(Uint16 data)
{
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPITXBUF = data;
    WAIT_FOR_SERIAL;
    dummy = SpiaRegs.SPIRXBUF;
}

Uint16 SPIBus_2 :: receiveWord(void) {
    SpiaRegs.SPICTL.bit.TALK = 0;
    SpiaRegs.SPITXBUF = dummy;
    WAIT_FOR_SERIAL;
    return SpiaRegs.SPIRXBUF & mask; // mask off if we're in 8-bit mode
}




