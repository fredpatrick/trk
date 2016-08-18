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

#include "Switch.h"
#include "SwitchSensor.h"
#include "DemuxAddress.h"
#include "EventDevice.h"
#include "GPIO.h"
#include <iostream>


trk::
Switch::Switch(int sw_num)
{
    if ( sw_num < 0 || sw_num > 5) {
        std::cout << "Switch.ctor, WARNING sw_num = " << sw_num << 
                                         " OUT OF RANGE" << std::endl;
        //throw ....
    }
    switch_sensor_thru_ = 0;
    switch_sensor_out_  = 0;

    demux_address_ = DemuxAddress::instance();

    gpio_config_ = GPIOConfig::instance();
//  std::cout << "Switch.ctor" << std::endl;
    sw_num_ = sw_num;
    SWKey keythru = {sw_num, THRU};
    gpio_thru_  = gpio_config_->switch_gpio(keythru);
    SWKey keyout = {sw_num, OUT};
    gpio_out_   = gpio_config_->switch_gpio(keyout);
//  std:cout << "Switch.ctor: Got the gpios" << endl;
}

trk::Switch::
~Switch()
{
//  std::cout << "Switch.dtor" << std::endl;
    if ( switch_sensor_thru_) switch_sensor_thru_->ignore_event(true);
    if ( switch_sensor_out_ ) switch_sensor_out_->ignore_event(true);
    delete gpio_thru_;
    delete gpio_out_;
}

bool
trk::Switch::
enable_sensors(EventDevice* efd, int& n_event)
{

    switch_sensor_thru_ = new SwitchSensor(sw_num_, 
                                           THRU, 
                                           efd, 
                                           n_event);
    gpio_thru_->edge_type(BOTH);
    gpio_thru_->debounce_time(200);
    gpio_thru_->wait_for_edge(switch_sensor_thru_);
//  std::cout << "Switch.ctor, Poll started on " << gpio_thru_->number() << 
//                                      " thru position" << endl;

    switch_sensor_out_ = new SwitchSensor(sw_num_, 
                                          OUT, 
                                          efd, 
                                          n_event);
    gpio_out_->edge_type(BOTH);
    gpio_out_->debounce_time(200);
    gpio_out_->wait_for_edge(switch_sensor_out_);
//  std::cout << "Switch.ctor, Poll started on " << gpio_out_->number() << 
//                                      " out position" << endl;
    return true;
}

bool
trk::Switch::
set_direction(const SW_DIRECTION& sw_direc)
{
    sw_direc_ = sw_direc;
    int addr = sw_num_ * 2 + sw_direc_;
    demux_address_->set(addr);
    return true;
}

void
trk::
Switch::sensor_event(int value, int count,const SW_DIRECTION& sw_direc)
{
    cout << "Switch, value = " << value << endl;
    cout << "Switch, count = " << count << endl;
}
    
trk::SW_DIRECTION
trk::Switch::
state()
{
    int sw_thru = gpio_thru_->value();
    int sw_out  = gpio_out_->value();

    if     ( sw_thru == 1 && sw_out == 0 ) return THRU;
    else if ( sw_thru == 0 && sw_out == 1 ) return OUT;
    else return NOVAL;
}

int 
trk::Switch::
sw_num()
{
    return sw_num_;
}
