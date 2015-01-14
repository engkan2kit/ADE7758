/**
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 *
 * This file is part of the panStamp project.
 *
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * panLoader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with panLoader; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 *
 * Author: Daniel Berenguer
 * Creation date: 03/03/2011
 */

#ifndef _SPI_H
#define _SPI_H

#include "Arduino.h"


#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR


/**
 * Macros
 */
// Wait until SPI operation is terminated
#define wait_Spi()  while(!(SPSR & _BV(SPIF)))

/**
 * Class: SPI
 *
 * Description:
 * Basic SPI class
 */
class SPI
{
  public:
    /**
     * init
     *
     * SPI initialization
     */
    void init();

    /**
     * send
     *
     * Send byte via SPI
     *
     * 'value'  Value to be sent
     *
     * Return:
     *  Response received from SPI slave
     */
    byte send(byte value);

    inline static byte transfer(byte _data, uint8_t mode);

    // SPI Configuration methods

    inline static void attachInterrupt();
    inline static void detachInterrupt(); // Default
    static void end();

    static void setBitOrder(uint8_t);
    static void setDataMode(uint8_t);
    static uint8_t getDataMode();
    static void setClockDivider(uint8_t);


};

extern SPI objSPI;

byte SPI::transfer(byte _data, uint8_t mode) {
  byte ret,prevmode;
  prevmode = getDataMode();
  setDataMode(mode);
  SPDR = _data;
  while (!(SPSR & _BV(SPIF)))
    ;
  ret = SPDR;
  setDataMode(prevmode);
  return ret;
}

#endif


