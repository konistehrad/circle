//
// kernel.cpp
//
#include "kernel.h"
#include "oledtask.h"

static const char FromKernel[] = "kernel";

CKernel::CKernel (void)
: m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
  m_Serial (&m_Interrupt, FALSE),
  m_Timer (&m_Interrupt),
  m_Logger (m_Options.GetLogLevel(), &m_Timer),
	m_Console (&m_Serial),
  m_I2CMaster (CMachineInfo::Get()->GetDevice(DeviceI2CMaster), TRUE)
{
}

CKernel::~CKernel (void)
{
}

void CKernel::dieWithBlinkPattern(uint8_t count)
{
  while(1)
  {
    m_ActLED.Blink(count, 200, 200);
    CTimer::SimpleMsDelay(2000);
  }
}

boolean CKernel::Initialize (void)
{
  boolean bOK = TRUE;

  if (bOK)
  {
    bOK = m_Interrupt.Initialize ();
  }
  if(!bOK) this->dieWithBlinkPattern(1);

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}
  if(!bOK) this->dieWithBlinkPattern(2);

  if (bOK)
  {
    bOK = m_Serial.Initialize (115200);
  }
  if(!bOK) this->dieWithBlinkPattern(3);
  
  if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}
  if(!bOK) this->dieWithBlinkPattern(4);

  if (bOK)
  {
    bOK = m_Timer.Initialize ();
  }
  if(!bOK) this->dieWithBlinkPattern(5);

	if (bOK)
	{
		bOK = m_Console.Initialize ();
	}
  if(!bOK) this->dieWithBlinkPattern(6);

  if (bOK)
  {
    bOK = m_I2CMaster.Initialize ();
  }
  if(!bOK) this->dieWithBlinkPattern(7);

  return bOK;
}

TShutdownMode CKernel::Run (void)
{
  m_Logger.Write(FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

  u8hal.i2cMaster = &m_I2CMaster;
  u8hal.timer = &m_Timer;
  u8hal.logger = &m_Logger;
  u8g2_SetUserPtr(&u8g2, &u8hal); // assign HAL to the user pointer
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_arm_circle_hw_i2c, u8x8_arm_circle_gpio_and_delay);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  m_Logger.Write(FromKernel, LogNotice, "u8 init complete!");

  COLEDTask* view = new COLEDTask(&u8g2);
  while(1) {
    m_Event.Clear ();
    view->Heartbeat();
    m_Timer.StartKernelTimer (1 * HZ, TimerHandler, this);
    m_Event.Wait ();
  }

  return ShutdownHalt;
}

void CKernel::TimerHandler (TKernelTimerHandle hTimer, void *pParam, void *pContext)
{
	CKernel *pThis = (CKernel *) pParam;
	assert (pThis != 0);
	pThis->m_Event.Set ();
}
