/*
 * ============================================================================
 *                   The XyloComp Software License, Version 1.1
 * ============================================================================
 * 
 *    Copyright (C) 2016 XyloComp Inc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modifica-
 * tion, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of  source code must  retain the above copyright  notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The end-user documentation included with the redistribution, if any, must
 *    include  the following  acknowledgment:  "This product includes  software
 *    developed  by  XyloComp Inc.  (http://www.xylocomp.com/)." Alternately, 
 *    this  acknowledgment may  appear in the software itself,  if
 *    and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The name "XyloComp" must not be used to endorse  or promote  products 
 *    derived  from this  software without  prior written permission. 
 *    For written permission, please contact fred.patrick@xylocomp.com.
 * 
 * 5. Products  derived from this software may not  be called "XyloComp", 
 *    nor may "XyloComp" appear  in their name,  without prior written 
 *    permission  of Fred Patrick
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS  FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED.  IN NO  EVENT SHALL
 * XYLOCOMP INC. OR ITS CONTRIBUTORS  BE LIABLE FOR  ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLU-
 * DING, BUT NOT LIMITED TO, PROCUREMENT  OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR  PROFITS; OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON
 * ANY  THEORY OF LIABILITY,  WHETHER  IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING  NEGLIGENCE OR  OTHERWISE) ARISING IN  ANY WAY OUT OF THE  USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef TRK_SWITCHDRIVER_HH
#define TRK_SWITCHDRIVER_HH

#include "inputsensor.h"
#include "trkutl.h"
#include <string>
#include <vector>
#include <utility>

namespace trk {

class InputGPIO;
class LayoutConfig;
class DebugCntl;
class JobClock;
class DemuxAddress;
class EventDevice;

class SwitchDriver  : public InputSensor 
{
    public:
        SwitchDriver(int sw_num);
        ~SwitchDriver();

        bool            enable_sensors(EventDevice* efd);
        void            event(int, InputGPIO*);
        void           set(int v);
        SW_DIRECTION    scan();
        int             sw_num();
        int             value() { return 0; }
        int             count() { return 0;}
        double          timeofday() { return 0.0; }
        
    private:
        LayoutConfig*   layout_config_;
        DebugCntl*      dbg_;
        JobClock*       job_clock_;
        int             sw_num_;
        SW_DIRECTION    state_;;
        std::string     switch_name_;
        InputGPIO*      gpio_thru_;
        InputGPIO*      gpio_out_;
        DemuxAddress*   demux_address_;
        EventDevice*    event_device_;
        bool            ignore_event_;
        double          tm_event_;
        int             event_count_;
        SW_DIRECTION    event_state_0_;
        SW_DIRECTION    event_state_1_;
        SW_DIRECTION    event_state(InputGPIO* gpio);
};

}

#endif
