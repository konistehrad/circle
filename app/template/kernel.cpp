//
// kernel.cpp
//

#include "kernel.h"
#define I2C_ADDRESS 0x3C

static const char FromKernel[] = "kernel";

CKernel::CKernel (void)
:	m_CPUThrottle (CPUSpeedMaximum),
  m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
  m_Timer (&m_Interrupt),
  m_Logger (m_Options.GetLogLevel (), &m_Timer)
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
    bOK = m_Interrupt.Initialize ();
  }

  if (bOK)
  {
    bOK = m_Timer.Initialize ();
  }

  // TODO: call Initialize () of added members here (if required)
  Wire.begin();
  Wire.setClock(400000L);

  return bOK;
}

TShutdownMode CKernel::Run (void)
{
  m_Logger.Write (FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

  m_Logger.Write (FromKernel, LogNotice, "Initializing OLED...");
  m_oled.begin(&Adafruit128x64, I2C_ADDRESS);
  m_oled.setFont(System5x7);
  m_Logger.Write (FromKernel, LogNotice, "OLED Initialization Complete!");

  // TODO: add your code here
  boolean state = false;
  u16 counter = 0;
  while(1)
  {
    m_oled.setRow(0);
    m_oled.setCol(0);
    m_oled.print("Loop: ");
    m_oled.println(counter);
    m_oled.print("Clock speed: ");
    m_oled.print(m_CPUThrottle.GetClockRate()/1000000);
    m_oled.print(" Mhz");
    
    state = !state;
    counter += 1;
  }

  return ShutdownHalt;
}
