
#include <iostream>

#include "Blocks.h"
#include "Block.h"

#include "DemuxAddress.h"
#include "GPIOConfig.h"

trk::Blocks::
Blocks()
{
    GPIOConfig* gpio_config = GPIOConfig::instance();
    blk_names_ = gpio_config-> blk_names();
    for ( int i = 0; i < blk_names_.size(); i++) {
        blocks_[ blk_names_[i] ] = new Block(blk_names_[i]);
    }

    int blk_base = gpio_config->blk_base_addr();
    DemuxAddress* demux_address = DemuxAddress::instance();
    demux_address->set(blk_base);
}

trk::Blocks::
~Blocks()
{
    for ( int i = 0; i < blk_names_.size(); i++) {
        delete blocks_[blk_names_[i] ];
    }
}

bool
trk::Blocks::
enable_sensors(int sensor_fd, int& n_event)
{
    for ( int i = 0; i < blocks_.size(); i++) {
        blocks_[ blk_names_[i] ]->enable_sensor(sensor_fd, n_event);
    }
    return true;
}

bool
trk::Blocks::
blockit( const std::string& blk_name)
{
    return blocks_[blk_name]->set();
}

bool
trk::Blocks::
clearit( const std::string& blk_name)
{
    return blocks_[blk_name]->clear();
}
