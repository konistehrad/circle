
#ifndef _U8G2LIB_CIRCLE_H
#define _U8G2LIB_CIRCLE_H


#define U8X8_USE_PINS
#define U8X8_WITH_USER_PTR

#include <circle/i2cmaster.h>
#include <circle/spimaster.h>
#include <circle/gpiomanager.h>
#include <circle/gpiopin.h>
#include <circle/timer.h>
#include <circle/logger.h>

#include "u8g2.h"
#include "u8x8.h"

struct CircleU8x8Hal
{
  CI2CMaster* i2cMaster;

  CSPIMaster* spiMaster;

  CGPIOManager* pinManager;
  CGPIOPin* pins[U8X8_PIN_CNT] = {NULL};

  CTimer* timer;
  CLogger* logger;

  CGPIOPin* GetPin(u8x8_t *u8x8, unsigned i) 
  { 
    // check out-of-range
    if(i >= U8X8_PIN_CNT) return NULL;
    // check empty pin bank
    auto idx = u8x8->pins[i];
    if(idx == U8X8_PIN_NONE) return NULL;
    if(idx >= U8X8_PIN_CNT) return NULL;
    return pins[idx];
  }
  void TryWrite(u8x8_t *u8x8, unsigned i, unsigned value)
  {
    auto pin = GetPin(u8x8, i);
    if(pin != NULL) pin->Write(value);
  }
};

extern "C" uint8_t u8x8_arm_circle_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern "C" uint8_t u8x8_byte_arm_circle_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif
