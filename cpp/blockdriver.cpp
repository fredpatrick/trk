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

#include "blockdriver.h"
#include "blocksensor.h"
#include "demuxaddress.h"
#include "eventdevice.h"
#include "gpio.h"
#include "layoutconfig.h"
#include <iostream>

trk::BlockDriver::
BlockDriver(const std::string& sensor_name)
{
    sensor_name_ = sensor_name;

    demux_address_ = DemuxAddress::instance();
    LayoutConfig* layout_config = LayoutConfig::instance();
    block_base_addr_            = layout_config->block_base_addr();
    block_sensor_index_         = layout_config->block_sensor_index(sensor_name);

    int gpio_num                = layout_config->block_sensor_gpio_num(sensor_name);
    gpio_                       = new InputGPIO(gpio_num);
    block_sensor_               = 0;
}

trk::BlockDriver::
~BlockDriver() 
{
    if (block_sensor_ ) block_sensor_->ignore_event();
    delete gpio_;
}

bool
trk::BlockDriver::
enable_sensor(EventDevice* efd)
{

    block_sensor_ = new BlockSensor(efd, sensor_name_);
    gpio_->edge_type(BOTH);
    gpio_->debounce_time(200);
    gpio_->wait_for_edge(block_sensor_);

    return true;
}

void
trk::BlockDriver::
set(int v)
{
    int i = scan();
    if ( i != v) {
        int block_addr = block_base_addr_ + 1 + block_sensor_index_;
        demux_address_->set(block_addr);
    }
}

trk::BLK_STATE
trk::BlockDriver::
scan()
{
    int v = gpio_->value();
    if ( v == 0 ) return GO;
    else          return STOP;
}

std::string
trk::BlockDriver::
sensor_name()
{
    return sensor_name_;
}
