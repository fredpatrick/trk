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

#include "layoutconfig.h"
#include "switchdriver.h"
#include "switchevent.h"
#include "demuxaddress.h"
#include "eventdevice.h"
#include "gpio.h"
#include "debugcntl.h"
#include "jobclock.h"
#include <iostream>
#include <unistd.h>

pthread_mutex_t write_event_ = PTHREAD_MUTEX_INITIALIZER;

trk::
SwitchDriver::SwitchDriver(int sw_num)
{
    dbg_           = DebugCntl::instance();
    job_clock_     = JobClock::instance();

    demux_address_ = DemuxAddress::instance();

    layout_config_ = LayoutConfig::instance();
    sw_num_        = sw_num;
    SWKey keythru  = {sw_num, THRU};
    switch_name_   = layout_config_-> switch_name(keythru);
    int gpio_num   = layout_config_->switch_sensor_gpio_num(keythru);
    gpio_thru_     = new InputGPIO(gpio_num, "both");
    SWKey keyout   = {sw_num, OUT};
    gpio_num       = layout_config_->switch_sensor_gpio_num(keyout);
    gpio_out_      = new InputGPIO(gpio_num, "both");
    scan();
    event_count_   = 0;
    ignore_event_  = false;
}

trk::SwitchDriver::
~SwitchDriver()
{
    ignore_event_ = true;
    delete gpio_thru_;
    delete gpio_out_;
}

bool
trk::SwitchDriver::
enable_sensors(EventDevice* efd)
{
    event_device_ = efd;
    SWKey key;
    key.num = sw_num_;
    key.swd = THRU;
    gpio_thru_->debounce_time(200);
    gpio_thru_->wait_for_edge(this);

    gpio_out_->debounce_time(200);
    gpio_out_->wait_for_edge(this);

    if (dbg_->check(2) ) {
        std::cout << "SwitchDriver.enable_sensors" << std::endl;
    }
    return true;
}

void
trk::SwitchDriver::
event(int ierr, InputGPIO* gpio)
{
    if ( ierr != 0 ) {
        std::cout << "SwitchDriver::event, ierr = " << ierr << std::endl;
        return;
    }

    if ( ignore_event_) return;

    int ier = pthread_mutex_lock(&write_event_);
    if ( ier != 0 ) {
        std::cout << "SwitchDriver.event, on mutxex lock, ier = " << ier << std::endl;
        return;
    }
    
    if      ( event_count_ == 0 ) event_state_0_ = event_state(gpio);
    else if ( event_count_ == 1 ) event_state_1_ = event_state(gpio);
    event_count_ += 1;
    if ( event_count_ == 2 ) {
        if ( event_state_0_ == event_state_1_ ) {
            state_       = event_state_0_;
            tm_event_    = job_clock_->job_time();
            SwitchEvent* sw_event = new SwitchEvent(tm_event_,
                                                    switch_name_,
                                                    sw_num_,
                                                    state_);
            sw_event->write_event(event_device_);
            sw_event->print(50);
        } else {
            if ( dbg_->check(0) ) {
                *dbg_ << "SwitchDriver.event, error receiving switch events, " << switch_name_
                                                             << trk::endl;
                *dbg_ << "                    event 0 = " << event_state_0_ <<
                               ", event 1 = " << event_state_1_ << trk::endl;
            }
        }
        event_count_ = 0;
    }
    ier = pthread_mutex_unlock(&write_event_);
}

trk::SW_DIRECTION
trk::SwitchDriver::
event_state(InputGPIO* gpio)
{
    int value = gpio->value();
    if      ( gpio == gpio_thru_ ) {
        std::cout << "SwitchDriver.event_state, gpio = thru, value = " << value << std::endl;
        if ( value == 1) return THRU;
        else             return OUT;
    } else if (gpio == gpio_out_ ) {
        std::cout << "SwitchDriver.event_state, gpio = out , value = " << value << std::endl;
        if ( value == 1) return OUT;
        else             return THRU;
    }
}
            
void
trk::SwitchDriver::
set(int v)
{
    int i = scan();
    if ( i != v) {
        int addr = sw_num_ * 2 + v;
        demux_address_->set(addr);
        if ( dbg_->check(2) ) {
            std::cout << "SwitchDriver.set, switch_name = " << switch_name_ << 
                                          ", i,v = " << i << ", " << v << std::endl;
        }
    }
}
    
trk::SW_DIRECTION
trk::SwitchDriver::
scan()
{
    int sw_thru = gpio_thru_->value();
    int sw_out  = gpio_out_->value();

    SW_DIRECTION sw_state;
    if     ( sw_thru == 1 && sw_out == 0 ) sw_state =  THRU;
    else if ( sw_thru == 0 && sw_out == 1 ) sw_state =  OUT;
    else sw_state =  NOVAL;
    if ( dbg_->check(2) ) {
        *dbg_ << "SwitchDriver.scan, i = " << sw_num_ << 
               ", thru_value = " << sw_thru << ", out_value = " << sw_out <<
               ", sw_state = " << sw_state << trk::endl;
    }
    return sw_state;
}

int 
trk::SwitchDriver::
sw_num()
{
    return sw_num_;
}
