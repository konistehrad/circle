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

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include <assert.h>
#include "Wire.h"
#include "twi.h"

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

TwoWire::TwoWire() : TwoWire(1) {}
TwoWire::TwoWire(uint8_t nDevice)
: m_nDevice (nDevice),
  m_nBaseAddress (nDevice == 0 ? ARM_BSC0_BASE : ARM_BSC1_BASE),
  m_SDA (nDevice == 0 ? 0 : 2, GPIOModeInput), // uninitialized to start
	m_SCL (nDevice == 0 ? 1 : 3, GPIOModeInput)
{
  twi_initialize(m_nBaseAddress);
}

void TwoWire::begin(void){
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;
  
  m_SDA.SetMode(GPIOModeAlternateFunction0);
  m_SCL.SetMode(GPIOModeAlternateFunction0);

  setClock(100000);
}

void TwoWire::end(void){
  m_SDA.SetMode(GPIOModeInput);
  m_SCL.SetMode(GPIOModeInput);
}

void TwoWire::setClock(uint32_t frequency){
  PeripheralEntry ();

	assert (frequency > 0);
	u16 nDivider = (u16) (m_nCoreClockRate / frequency);
	write32 (m_nBaseAddress + ARM_BSC_DIV__OFFSET, nDivider);
	
	PeripheralExit ();
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop){
  if(quantity > BUFFER_LENGTH) quantity = BUFFER_LENGTH;
  uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
  rxBufferIndex = 0;
  rxBufferLength = (read == 0)?quantity:0;
  return rxBufferLength;
}

void TwoWire::beginTransmission(uint8_t address){
  txAddress = address;
  txBufferIndex = 0;
  txBufferLength = 0;
}

uint8_t TwoWire::endTransmission(uint8_t sendStop){
  uint8_t ret = twi_writeTo(txAddress, txBuffer, txBufferLength, 0, sendStop);
  txBufferIndex = 0;
  txBufferLength = 0;
  return ret;
}

uint8_t TwoWire::endTransmission(void){
  return endTransmission(true);
}

size_t TwoWire::write(uint8_t data){
  if(txBufferLength >= BUFFER_LENGTH) return 0;
  txBuffer[txBufferIndex] = data;
  ++txBufferIndex;
  txBufferLength = txBufferIndex;
  return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity){
  for(size_t i = 0; i < quantity; ++i){
    write(data[i]);
  }
  return quantity;
}

int TwoWire::available(void){
  return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void){
  int value = -1;
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  return value;
}

int TwoWire::peek(void){
  int value = -1;
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }
  return value;
}

void TwoWire::begin(uint8_t address){
  return;
  // begin();
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity){
  return requestFrom(address, quantity, (uint8_t)true);
}

TwoWire Wire = TwoWire();
