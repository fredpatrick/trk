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

#include "Block.h"
#include "BlockSensor.h"
#include "DemuxAddress.h"
#include "EventDevice.h"
#include "GPIO.h"
#include "GPIOConfig.h"
#include <iostream>

trk::Block::
Block(const std::string& blk_name)
{
    blk_name_ = blk_name;

    demux_address_ = DemuxAddress::instance();
    GPIOConfig* gpio_config    = GPIOConfig::instance();
    blk_base_addr_ = gpio_config->blk_base_addr();
    blk_index_     = gpio_config->blk_index(blk_name);

    gpio_           = gpio_config->blk_gpio(blk_name);
    blk_sensor_     = 0;
}

trk::Block::
~Block() 
{
    if (blk_sensor_ ) blk_sensor_->ignore_event();
    delete gpio_;
}

bool
trk::Block::
enable_sensor(EventDevice* efd, 
              int&         n_event)
{

    blk_sensor_ = new BlockSensor(efd, n_event, blk_name_);
    gpio_->edge_type(BOTH);
    gpio_->debounce_time(200);
    gpio_->wait_for_edge(blk_sensor_);

    return true;
}

bool 
trk::Block::
set()
{
    int blk_addr = blk_base_addr_ + 1 + blk_index_;
    demux_address_->set(blk_addr);
}

bool 
trk::Block::
clear()
{
    int blk_addr = blk_base_addr_ + 1 + blk_index_;
    demux_address_->set(blk_addr);
}

trk::BLK_STATE
trk::Block::
state()
{
    int v = gpio_->value();
    if ( v == 0 ) return GO;
    else          return STOP;
}

std::string
trk::Block::
blk_name()
{
    return blk_name_;
}
