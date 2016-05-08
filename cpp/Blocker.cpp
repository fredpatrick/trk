


#include "Blocker.h"
#include "DemuxAddress.h"
#include "GPIOConfig.h"
#include <iostream>

trk::Blocker::
Blocker(const std::string& blk_name)
{
    blk_name_ = blk_name;

    demux_address_ = DemuxAddress::instance();
    GPIOConfig* gpio_config    = GPIOConfig::instance();
    blk_base_addr_ = gpio_config->blk_base_addr();
    blk_index_     = gpio_config->blk_index(blk_name);
}

trk::Blocker::
~Blocker() 
{
}

bool 
trk::Blocker::
set()
{
    int blk_addr = blk_base_addr_ + 1 + blk_index_;
    demux_address_->set(blk_addr);
}

bool 
trk::Blocker::
clear()
{
    int blk_addr = blk_base_addr_ + 1 + blk_index_;
    demux_address_->set(blk_addr);
}
