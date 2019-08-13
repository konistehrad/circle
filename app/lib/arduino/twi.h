#ifndef twi_h
#define twi_h

#include <inttypes.h>
#include <circle/types.h>
#include <circle/bcm2835.h>

#define _BV(a) (1 << (a))

#define BCM2835_REG(a, b) *(((volatile uint32_t *)((a) + ((b) / 4))))

#define BSCI2CEN    15 // I2C Enable, 0 = disabled, 1 = enabled
#define BSCINTR     10 // Interrupt on RX
#define BSCINTT     9 /// Interrupt on TX
#define BSCINTD     8 /// Interrupt on DONE
#define BSCST       7 /// Start transfer, 1 = Start a new transfer
#define BSCCLEAR    4 /// Clear FIFO Clear
#define BSCREAD     0 /// Read transfer

//BSCxS bits
#define BSCCLKT      9 // Clock stretch timeout
#define BSCERR       8 // ACK error
#define BSCRXF       7 // RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BSCTXE       6 // TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BSCRXD       5 // RXD FIFO contains data
#define BSCTXD       4 // TXD FIFO can accept data
#define BSCRXR       3 // RXR FIFO needs reading (full)
#define BSCTXW       2 // TXW FIFO needs writing (full)
#define BSCDONE      1 // Transfer DONE
#define BSCTA        0 // Transfer Active

//BSC0 is on pins 0 and 1 and is used for HAT I2C EEPROM
//BSC1 is on pin 2 and pin 3 and is accessible by the Wire library
// Defines for BSC0 and BSC1 I2C Master interfaces
#define BSC0C       BCM2835_REG(ARM_BSC0_BASE, 0x0000) ///< BSC0 Master Control
#define BSC0S       BCM2835_REG(ARM_BSC0_BASE, 0x0004) ///< BSC0 Master Status
#define BSC0DLEN    BCM2835_REG(ARM_BSC0_BASE, 0x0008) ///< BSC0 Master Data Length
#define BSC0A       BCM2835_REG(ARM_BSC0_BASE, 0x000c) ///< BSC0 Master Slave Address
#define BSC0FIFO    BCM2835_REG(ARM_BSC0_BASE, 0x0010) ///< BSC0 Master Data FIFO
#define BSC0DIV      BCM2835_REG(ARM_BSC0_BASE, 0x0014) ///< BSC0 Master Clock Divider
#define BSC0DEL      BCM2835_REG(ARM_BSC0_BASE, 0x0018) ///< BSC0 Master Data Delay
#define BSC0CLKT    BCM2835_REG(ARM_BSC0_BASE, 0x001c) ///< BSC0 Master Clock Stretch Timeout

#define BSC1C       BCM2835_REG(ARM_BSC1_BASE, 0x0000) ///< BSC0 Master Control
#define BSC1S       BCM2835_REG(ARM_BSC1_BASE, 0x0004) ///< BSC0 Master Status
#define BSC1DLEN    BCM2835_REG(ARM_BSC1_BASE, 0x0008) ///< BSC0 Master Data Length
#define BSC1A       BCM2835_REG(ARM_BSC1_BASE, 0x000c) ///< BSC0 Master Slave Address
#define BSC1FIFO    BCM2835_REG(ARM_BSC1_BASE, 0x0010) ///< BSC0 Master Data FIFO
#define BSC1DIV      BCM2835_REG(ARM_BSC1_BASE, 0x0014) ///< BSC0 Master Clock Divider
#define BSC1DEL      BCM2835_REG(ARM_BSC1_BASE, 0x0018) ///< BSC0 Master Data Delay
#define BSC1CLKT    BCM2835_REG(ARM_BSC1_BASE, 0x001c) ///< BSC0 Master Clock Stretch Timeout

#define BSCF2DIV(f)     (((150000000/f) + 1) & 0xFFFE)
#define BSCDIV2F(d)     (150000000/d)
#define BSC0FREQ    (BSC0DIV?BSCDIV2F(BSC0DIV):0)
#define BSC1FREQ    (BSC1DIV?BSCDIV2F(BSC1DIV):0)

#ifdef __cplusplus
extern "C"{
#endif

extern uint8_t TWBR;

uint8_t twi_readFrom(uint8_t, uint8_t*, uint32_t, uint8_t);
uint8_t twi_writeTo(uint8_t, uint8_t*, uint32_t, uint8_t, uint8_t);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
