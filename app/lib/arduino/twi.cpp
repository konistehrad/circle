/*
  I2C Master library for Raspberry Pi Arduino.
  Copyright (c) 2015 by Hristo Gochkov

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "twi.h"
#include <circle/spinlock.h>

// Control register
#define C_I2CEN			(1 << 15)
#define C_INTR			(1 << 10)
#define C_INTT			(1 << 9)
#define C_INTD			(1 << 8)
#define C_ST			(1 << 7)
#define C_CLEAR			(1 << 5)
#define C_READ			(1 << 0)

// Status register
#define S_CLKT			(1 << 9)
#define S_ERR			(1 << 8)
#define S_RXF			(1 << 7)
#define S_TXE			(1 << 6)
#define S_RXD			(1 << 5)
#define S_TXD			(1 << 4)
#define S_RXR			(1 << 3)
#define S_TXW			(1 << 2)
#define S_DONE			(1 << 1)
#define S_TA			(1 << 0)

// FIFO register
#define FIFO__MASK		0xFF
#define FIFO_SIZE		16

uint8_t TWBR;

static CSpinLock m_SpinLock(TASK_LEVEL);
static uint32_t m_nBaseAddress = ARM_BSC1_BASE;
void twi_initialize(unsigned baseAddress){
  m_nBaseAddress = baseAddress;
}

uint8_t twi_writeTo(uint8_t address, uint8_t * buf, uint32_t len, uint8_t var, uint8_t sendStop){
  uint32_t remaining = len;
  uint32_t i = 0;
  uint8_t reason = 0;

	m_SpinLock.Acquire ();
	PeripheralEntry ();

	write32 (m_nBaseAddress + ARM_BSC_A__OFFSET, address);
	write32 (m_nBaseAddress + ARM_BSC_C__OFFSET, C_CLEAR);
	write32 (m_nBaseAddress + ARM_BSC_S__OFFSET, S_CLKT | S_ERR | S_DONE);
	write32 (m_nBaseAddress + ARM_BSC_DLEN__OFFSET, len);

  while(remaining && ( i < 16 )){
    write32 (m_nBaseAddress + ARM_BSC_FIFO__OFFSET, buf[i]);
    i++;
    remaining--;
  }

  write32 (m_nBaseAddress + ARM_BSC_C__OFFSET, C_I2CEN | C_ST);
  while(!(read32 (m_nBaseAddress + ARM_BSC_S__OFFSET) & S_DONE)){
    while(remaining && (read32 (m_nBaseAddress + ARM_BSC_S__OFFSET) & S_TXD)){
      write32 (m_nBaseAddress + ARM_BSC_FIFO__OFFSET, buf[i]);
      i++;
      remaining--;
    }
  }

  u32 nStatus = read32 (m_nBaseAddress + ARM_BSC_S__OFFSET);
  if (nStatus & S_ERR){
		write32 (m_nBaseAddress + ARM_BSC_S__OFFSET, S_ERR);
    reason = 1;
  } else if (nStatus & S_CLKT){
    reason = 2;
  } else if (remaining){
    reason = 3;
  }

  write32 (m_nBaseAddress + ARM_BSC_S__OFFSET, S_DONE);
  PeripheralExit ();
	m_SpinLock.Release ();

  return reason;
}

uint8_t twi_readFrom(uint8_t address, uint8_t* buf, uint32_t len, uint8_t sendStop){
  uint32_t remaining = len;
  uint32_t i = 0;
  uint8_t reason = 0;

  m_SpinLock.Acquire ();
	assert (buf != 0);
	PeripheralEntry ();

  write32 (m_nBaseAddress + ARM_BSC_A__OFFSET, address);
  write32 (m_nBaseAddress + ARM_BSC_C__OFFSET, C_CLEAR);
  write32 (m_nBaseAddress + ARM_BSC_S__OFFSET, S_CLKT | S_ERR | S_DONE);
  write32 (m_nBaseAddress + ARM_BSC_DLEN__OFFSET, len);
  write32 (m_nBaseAddress + ARM_BSC_C__OFFSET, C_I2CEN | C_ST | C_READ);
  
  while(!(read32 (m_nBaseAddress + ARM_BSC_S__OFFSET) & S_DONE)){
    while (read32 (m_nBaseAddress + ARM_BSC_S__OFFSET) & S_RXD){
      buf[i] = read32 (m_nBaseAddress + ARM_BSC_FIFO__OFFSET) & FIFO__MASK;
      i++;
      remaining--;
    }
  }
  
  while (remaining && (read32 (m_nBaseAddress + ARM_BSC_S__OFFSET) & S_RXD)){
    buf[i] = read32 (m_nBaseAddress + ARM_BSC_FIFO__OFFSET) & FIFO__MASK;
    i++;
    remaining--;
  }
  
	u32 nStatus = read32 (m_nBaseAddress + ARM_BSC_S__OFFSET);
  if (nStatus & S_ERR){
    write32 (m_nBaseAddress + ARM_BSC_S__OFFSET, S_ERR);
    reason = 1;
  } else if (nStatus & S_CLKT){
    reason = 2;
  } else if (remaining){
    reason = 3;
  }
  write32 (m_nBaseAddress + ARM_BSC_S__OFFSET, S_DONE);
	PeripheralExit ();
	m_SpinLock.Release ();
  return reason;
}
