//
// kernel.cpp
//
#include "kernel.h"

static const char FromKernel[] = "kernel";

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
#if RASPPI >= 4 && AARCH == 64
	m_Serial (&m_Interrupt, FALSE),		// FIQ not supported
#else
	m_Serial (&m_Interrupt, TRUE),
#endif
	m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel (), &m_Timer),
	m_USBHCI (&m_Interrupt, &m_Timer),
	m_Console (&m_Serial),
	m_I2CMaster (CMachineInfo::Get ()->GetDevice (DeviceI2CMaster))
{
}

CKernel::~CKernel (void)
{
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	if (bOK)
	{
		bOK = m_USBHCI.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Console.Initialize ();
	}

	if (bOK)
	{
		bOK = m_I2CMaster.Initialize ();
	}

	if (bOK)
	{
		u8hal.i2cMaster = &m_I2CMaster;
		u8hal.timer = &m_Timer;
		u8g2_SetUserPtr(&u8g2, &u8hal); // assign HAL to the user pointer
		u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_arm_circle_hw_i2c, u8x8_arm_circle_gpio_and_delay);
		u8g2_InitDisplay(&u8g2);
		u8g2_SetPowerSave(&u8g2, 0);
	}

	return bOK;
}

TShutdownMode CKernel::Run (void)
{
	m_Logger.Write (FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

	if (m_Console.IsAlternateDeviceUsed ())
	{
		m_Logger.Write (FromKernel, LogNotice, "Using serial interface");
	}

	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
	u8g2_DrawStr(&u8g2, 1, 18, "U8g2 on Circle");
	u8g2_SendBuffer(&u8g2);

	u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
	u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603 );
	u8g2_SendBuffer(&u8g2);

	while(1) {
		m_ActLED.Blink(100, 1000, 1000);
	}

	return ShutdownHalt;
}
