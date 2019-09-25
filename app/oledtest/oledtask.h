//
// oledtask.h
//
#ifndef _oledtask_h
#define _oledtask_h

#include <circle/timer.h>
#include <circle/sched/task.h>
#include <circle/cputhrottle.h>
#include <circle/string.h>
#include <u8g2/u8g2.h>

class COLEDTask : public CTask
{
public:
  COLEDTask (u8g2_t* u8g2, CCPUThrottle* throttle);
  ~COLEDTask (void);

  void Heartbeat ();
  void Run (void);

private:
  boolean m_hbState;
  u8g2_t* m_u8g2;
  CCPUThrottle* m_throttle;
  CString m_formatter;
};


#endif
