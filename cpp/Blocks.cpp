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

#include "Blocks.h"
#include "Block.h"

#include "DemuxAddress.h"
#include "EventDevice.h"
#include "GPIOConfig.h"

trk::Blocks::
Blocks()
{
    GPIOConfig* gpio_config = GPIOConfig::instance();
    blk_names_ = gpio_config-> blk_names();
    for ( int i = 0; i < blk_names_.size(); i++) {
        Block* b = new Block(blk_names_[i]);
        blocks_.push_back(b);
        block_indexes_[blk_names_[i]] = i;
    }

    int blk_base = gpio_config->blk_base_addr();
    DemuxAddress* demux_address = DemuxAddress::instance();
    demux_address->set(blk_base);                             // CLR to J-K flipflop
}

trk::Blocks::
~Blocks()
{
    for ( int i = 0; i < blk_names_.size(); i++) {
        delete blocks_[i];
    }
}

bool
trk::Blocks::
enable_sensors(EventDevice* efd, int& n_event)
{
    for ( int i = 0; i < blocks_.size(); i++) {
        blocks_[ i ]->enable_sensor(efd, n_event);
    }
    return true;
}

bool
trk::Blocks::
blockit( const std::string& blk_name)
{
    int i = block_indexes_[blk_name];
    return blocks_[i]->set();
}

bool
trk::Blocks::
clearit( const std::string& blk_name)
{
    int i = block_indexes_[blk_name];
    return blocks_[i]->clear();
}

int
trk::Blocks::
n_block() const
{ 
    return blocks_.size();
}

std::string
trk::Blocks::
blk_name(int i) const
{
    return blk_names_[i];
}

trk::BLK_STATE
trk::Blocks::
get_state(int i) const
{
    return blocks_[ i ]->state();
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::Blocks& blocks)
{
    int n = blocks.n_block();
    for ( int i = 0; i < n; i++) ostrm << blocks.get_state(i);
    return ostrm;
}
