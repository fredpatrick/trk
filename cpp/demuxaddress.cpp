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

#include <iostream>

#include "demuxaddress.h"
#include "layoutconfig.h"

trk::DemuxAddress* trk::DemuxAddress::instance_ = 0;

trk::DemuxAddress*
trk::
DemuxAddress::instance()
{
    if( !instance_ ) {
        instance_ = new DemuxAddress();
    }
    return instance_;
}

trk::
DemuxAddress::DemuxAddress()
{
    LayoutConfig* layout_config = LayoutConfig::instance();
    int gpio_num = layout_config->demux_address_gpio_num("a0");
    gpios_[0] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a1");
    gpios_[1] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a2");
    gpios_[2] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a3");
    gpios_[3] = new OutputGPIO(gpio_num);
    gpio_num  = layout_config->demux_address_gpio_num("a4");
    gpios_[4] = new OutputGPIO(gpio_num);
}

trk::DemuxAddress::
~DemuxAddress()
{
    for ( int i = 0; i < 5; i++) {
        delete gpios_[i];
    }
    instance_ = 0;
}

bool
trk::
DemuxAddress::set(int addr)
{
    std::bitset<8 * sizeof(int)> b = addr;
    gpios_[4]->value(1);
    gpios_[0]->value( (int)b[0] );
    gpios_[1]->value( (int)b[1] );
    gpios_[2]->value( (int)b[2] );
    gpios_[3]->value( (int)b[3] );
    gpios_[4]->value(0);

    usleep(100000);
    gpios_[4]->value(1);
    return true;
}
