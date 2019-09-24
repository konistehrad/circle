//
// kernel.h
//
#ifndef _kernel_h
#define _kernel_h

#define U8X8_USE_PINS
#define U8X8_WITH_USER_PTR

#include <circle/memory.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/usb/usbhcidevice.h>
#include <circle/input/console.h>
#include <circle/i2cmaster.h>
#include <circle/types.h>
#include <u8g2/u8g2.h>
#include <u8g2/U8g2lib.h>

enum TShutdownMode
{
	ShutdownNone,
	ShutdownHalt,
	ShutdownReboot
};

class CKernel
{
public:
	CKernel (void);
	~CKernel (void);

	boolean Initialize (void);

	TShutdownMode Run (void);

private:
	CMemorySystem		m_Memory;
	CActLED			m_ActLED;
	CKernelOptions		m_Options;
	CDeviceNameService	m_DeviceNameService;
	CExceptionHandler	m_ExceptionHandler;
	CInterruptSystem	m_Interrupt;
	CScreenDevice		m_Screen;
	CSerialDevice		m_Serial;
	CTimer			m_Timer;
	CLogger			m_Logger;
	CUSBHCIDevice		m_USBHCI;

	CConsole		m_Console;
	CI2CMaster	m_I2CMaster;

  u8g2_t u8g2;
	CircleU8x8Hal u8hal;
};

#endif
