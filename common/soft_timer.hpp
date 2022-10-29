/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file soft_timer.hpp
 * @author d.ponomarev
 * @date Jun 24, 2018
 */

#ifndef __SOFT_TIMER_HPP
#define __SOFT_TIMER_HPP

#include "stdint.h"


/**
* @brief Software stopwatch/timer
*/
class SoftTimer
{
    public:
        enum TimerStatus_t
        {
            CREATED,
            WORKING,
            WAITING,
            STOPPED,
            FINISHED,
        };
        SoftTimer(uint32_t sys_clock);
        void StartUs(uint16_t timeUs);
        void StartMs(uint16_t timeMs);
        void Continue();
        void Wait();
        void Stop();
        TimerStatus_t GetStatus();
        uint32_t GetRestTime();
        uint32_t GetElapsedTime();
    private:
        bool IsTimerEnd() const;
    
        uint8_t  StartOverflows;
        uint8_t  RestOverflows;
        uint8_t  EndOverflows;
        
        uint32_t StartCount;
        uint32_t RestCount;
        uint32_t EndCount;
    
        TimerStatus_t Status;
        uint32_t ONE_MS;
        uint32_t ONE_S;
        uint32_t PERIOD;
};


#endif //__TIMER_HPP
