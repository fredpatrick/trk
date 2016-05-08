
#include <iostream>

#include "Blockers.h"
#include "Blocker.h"

#include "DemuxAddress.h"
#include "GPIOConfig.h"

trk::Blockers::
Blockers()
{
    GPIOConfig* gpio_config = GPIOConfig::instance();
    blk_names_ = gpio_config-> blk_names();
    for ( int i = 0; i < blk_names_.size(); i++) {
        blockers_[ blk_names_[i] ] = new Blocker(blk_names_[i]);
    }

    int blk_base = gpio_config->blk_base_addr();
    DemuxAddress* demux_address = DemuxAddress::instance();
    demux_address->set(blk_base);
}

trk::Blockers::
~Blockers()
{
    for ( int i = 0; i < blk_names_.size(); i++) {
        delete blockers_[blk_names_[i] ];
    }
}

bool
trk::Blockers::
blockit( const std::string& blk_name)
{
    return blockers_[blk_name]->set();
}

bool
trk::Blockers::
clearit( const std::string& blk_name)
{
    return blockers_[blk_name]->clear();
}
