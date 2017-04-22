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
#include "switchsensor.h"
#include "demuxaddress.h"
#include "eventdevice.h"
#include "gpio.h"
#include "debugcntl.h"
#include <iostream>


trk::
SwitchDriver::SwitchDriver(int sw_num)
{
    if ( sw_num < 0 || sw_num > 5) {
        std::cout << "SwitchDriver.ctor, WARNING sw_num = " << sw_num << 
                                         " OUT OF RANGE" << std::endl;
        //throw ....
    }
    dbg_ = DebugCntl::instance();

    switch_sensor_thru_ = 0;
    switch_sensor_out_  = 0;

    demux_address_ = DemuxAddress::instance();

    layout_config_ = LayoutConfig::instance();
    sw_num_        = sw_num;
    SWKey keythru  = {sw_num, THRU};
    switch_name_   = layout_config_-> switch_name(keythru);
    int gpio_num   = layout_config_->switch_sensor_gpio_num(keythru);
    gpio_thru_     = new InputGPIO(gpio_num);
    SWKey keyout   = {sw_num, OUT};
    gpio_num       = layout_config_->switch_sensor_gpio_num(keyout);
    gpio_out_      = new InputGPIO(gpio_num);
}

trk::SwitchDriver::
~SwitchDriver()
{
//  std::cout << "SwitchDriver.dtor" << std::endl;
    if ( switch_sensor_thru_) switch_sensor_thru_->ignore_event(true);
    if ( switch_sensor_out_ ) switch_sensor_out_->ignore_event(true);
    delete gpio_thru_;
    delete gpio_out_;
}

bool
trk::SwitchDriver::
enable_sensors(EventDevice* efd)
{
    SWKey key;
    key.num = sw_num_;
    key.swd = THRU;
    switch_sensor_thru_     = new SwitchSensor(switch_name_,
                                               sw_num_, 
                                               THRU, 
                                               efd);
    gpio_thru_->edge_type(BOTH);
    gpio_thru_->debounce_time(200);
    gpio_thru_->wait_for_edge(switch_sensor_thru_);
//  std::cout << "SwitchDriver.ctor, Poll started on " << gpio_thru_->number() << 
//                                      " thru position" << endl;

    switch_sensor_out_ = new SwitchSensor(switch_name_,
                                          sw_num_, 
                                          OUT, 
                                          efd);
    gpio_out_->edge_type(BOTH);
    gpio_out_->debounce_time(200);
    gpio_out_->wait_for_edge(switch_sensor_out_);

    if (dbg_->check(2) ) {
        std::cout << "SwitchDriver.enable_sensors" << std::endl;
    }
    return true;
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

void
trk::
SwitchDriver::sensor_event(int value, int count,const SW_DIRECTION& sw_direc)
{
    std::cout << "SwitchDriver, value = " << value << std::endl;
    std::cout << "SwitchDriver, count = " << count << std::endl;
}
    
trk::SW_DIRECTION
trk::SwitchDriver::
scan()
{
    int sw_thru = gpio_thru_->value();
    int sw_out  = gpio_out_->value();

    if     ( sw_thru == 1 && sw_out == 0 ) return THRU;
    else if ( sw_thru == 0 && sw_out == 1 ) return OUT;
    else return NOVAL;
}

int 
trk::SwitchDriver::
sw_num()
{
    return sw_num_;
}
