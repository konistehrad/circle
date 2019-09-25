#include "U8g2lib.h"

static const char FromU8G2[] = "u8g2";

uint8_t u8x8_arm_circle_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    CircleU8x8Hal* hal = (CircleU8x8Hal*)u8x8_GetUserPtr(u8x8);
    switch(msg)
    {
        case U8X8_MSG_DELAY_NANO:            // delay arg_int * 1 nano second
            // TODO: find nanosecond calculation for Circle
            hal->timer->usDelay(arg_int == 0 ? 0 : 1);
            break;    
    
        case U8X8_MSG_DELAY_100NANO:        // delay arg_int * 100 nano seconds
            // TODO: find nanosecond calculation for Circle
            hal->timer->usDelay(arg_int == 0 ? 0 : 1);
            break;
  
        case U8X8_MSG_DELAY_10MICRO:        // delay arg_int * 10 micro seconds
            hal->timer->usDelay(arg_int * 10);
            break;
  
        case U8X8_MSG_DELAY_MILLI:            // delay arg_int * 1 milli second
            hal->timer->MsDelay(arg_int);
            break;
  
        case U8X8_MSG_GPIO_AND_DELAY_INIT:  
            for( unsigned i = 0; i < U8X8_PIN_CNT; i++ )
            {
                if ( u8x8->pins[i] != U8X8_PIN_NONE )
                {
                    CGPIOPin* circlePin = hal->GetPin(u8x8, i);
                    if(circlePin == NULL) continue;

                    if ( i < U8X8_PIN_OUTPUT_CNT )
                    {
                        circlePin->SetMode(GPIOModeOutput);
                    }
                    else
                    {
                        circlePin->SetMode(GPIOModeOutput);
                        circlePin->Write(HIGH);
                    }
                }
            }
            break;
  
        case U8X8_MSG_GPIO_CS:
            hal->TryWrite(u8x8, U8X8_PIN_CS, arg_int ? HIGH : LOW);
            break;
  
        case U8X8_MSG_GPIO_DC:
            hal->TryWrite(u8x8, U8X8_PIN_DC, arg_int ? HIGH : LOW);
            break;  
  
        case U8X8_MSG_GPIO_RESET:
            hal->TryWrite(u8x8, U8X8_PIN_RESET, arg_int ? HIGH : LOW);
            break;
  
        default:
            return 0;
    }
    return 1;
}

uint8_t u8x8_byte_arm_circle_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buf_idx;
    uint8_t *data;
    CircleU8x8Hal* hal = (CircleU8x8Hal*)u8x8_GetUserPtr(u8x8);

    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;      
            while( arg_int > 0 )
            {
                buffer[buf_idx++] = *data;
                data++;
                arg_int--;
            }      
            break;
        case U8X8_MSG_BYTE_INIT:
            /* add your custom code to init i2c subsystem */
            break;
        case U8X8_MSG_BYTE_SET_DC:
            /* ignored for i2c */
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            buf_idx = 0;
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            hal->i2cMaster->Write(u8x8_GetI2CAddress(u8x8) >> 1, buffer, buf_idx);
            break;
        default:
            return 0;
    }
    return 1;
}
