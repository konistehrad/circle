//
// screentask.h
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2019  Conrad Kreyling <konistehrad@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "oledtask.h"
#include <circle/sched/scheduler.h>

#define FPS (30)
#define FRAME_MS (1000/FPS)
#define TICKS_PER_FRAME ((1000000 / 1000) * FRAME_MS)

COLEDTask::COLEDTask (u8g2_t* u8g2, CCPUThrottle* throttle)
: m_hbState(false), 
  m_u8g2(u8g2),
  m_throttle(throttle),
  m_formatter()
{
}

COLEDTask::~COLEDTask()
{
}

void COLEDTask::Heartbeat()
{
  m_hbState = !m_hbState;
}

void COLEDTask::Run()
{
  while(1)
  {
    uint8_t drawWidth = 0;
    uint8_t lineY = 0;
    unsigned startTicks = CTimer::GetClockTicks();

    u8g2_ClearBuffer(m_u8g2);
    u8g2_SetFont(m_u8g2, u8g2_font_helvR10_tr);

    lineY = 15;
    drawWidth = u8g2_DrawStr(m_u8g2, 1, lineY, "U8G2 on Circle");

    u8g2_SetFont(m_u8g2, u8g2_font_helvR08_tr);
    lineY = 25;
    m_formatter.Format("%d", m_throttle->GetClockRate());
    drawWidth = u8g2_DrawStr(m_u8g2, 1, lineY, "Clock speed: ");
    drawWidth += u8g2_DrawStr(m_u8g2, drawWidth, lineY, m_formatter);

    lineY = 35;
    m_formatter.Format("%d", m_throttle->GetTemperature());
    drawWidth = u8g2_DrawStr(m_u8g2, 1, lineY, "Temperature: ");
    drawWidth += u8g2_DrawStr(m_u8g2, drawWidth, lineY, m_formatter);
    
    lineY = 45;
    m_formatter.Format("%d", CTimer::Get()->GetUptime());
    drawWidth = u8g2_DrawStr(m_u8g2, 1, lineY, "Uptime (s): ");
    drawWidth += u8g2_DrawStr(m_u8g2, drawWidth, lineY, m_formatter);

    // u8g2_DrawFilledEllipse(m_u8g2, 40, 40, 10, 10, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_LOWER_LEFT);
    // u8g2_DrawEllipse(m_u8g2, 40, 40, 10, 10, U8G2_DRAW_LOWER_RIGHT);

    if(m_hbState)
    {
      u8g2_DrawBox(m_u8g2, m_u8g2->width - 2, m_u8g2->height - 2, 2, 2);
    }
    u8g2_SendBuffer(m_u8g2);

    unsigned endTicks = CTimer::GetClockTicks();
    unsigned sleepTicks = 
      endTicks < startTicks ? 
        (TICKS_PER_FRAME / 2) : //wraparound case
        (TICKS_PER_FRAME - (endTicks - startTicks));

    CScheduler::Get()->usSleep(sleepTicks);
  }
}

