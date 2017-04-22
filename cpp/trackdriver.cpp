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

#include "trackdriver.h"
#include "tracksensor.h"
#include "eventdevice.h"
#include "gpio.h"
#include "layoutconfig.h"
#include "debugcntl.h"
#include <iostream>


trk::
TrackDriver::TrackDriver(const std::string& sensor_name)
{
    track_sensor_ = 0;

    sensor_name_ = sensor_name;
    LayoutConfig* layout_config = LayoutConfig::instance();
    dbg_ = DebugCntl::instance();
    int gpio_num = layout_config->track_sensor_gpio_num(sensor_name_);
    sensor_index_ = layout_config->track_sensor_index(sensor_name_);
    track_gpio_  = new InputGPIO(gpio_num);
    track_sensor_ = 0;
}

trk::TrackDriver::
~TrackDriver()
{
//  std::cout << "TrackDriver.dtor" << std::endl;
    if ( track_sensor_) track_sensor_->ignore_event(true);
    delete track_gpio_;
}

bool
trk::TrackDriver::
enable_sensor(EventDevice* efd)
{

    track_sensor_= new TrackSensor(efd, sensor_name_, sensor_index_);
    track_gpio_->edge_type(BOTH);
    track_gpio_->debounce_time(200);
    track_gpio_->wait_for_edge(track_sensor_);

    if (dbg_->check(2) ) {
        std::cout << "TrackDriver.enable_sensor, name = " << sensor_name_ << std::endl;
    }

    return true;
}

trk::TRK_STATE
trk::TrackDriver::
scan()
{
    int v = track_gpio_->value();
    if ( v == 0 ) return IDLE;
    else          return BUSY;
}

std::string
trk::TrackDriver::
sensor_name()
{
    return sensor_name_;
}
