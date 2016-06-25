
#include "Block.h"
#include "BlockSensor.h"
#include "DemuxAddress.h"
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
enable_sensor(int sensor_fd, int& n_event)
{

    blk_sensor_ = new BlockSensor( sensor_fd, n_event, blk_name_);
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
    int v = (int)gpio_->value();
    if ( v == 0 ) return GO;
    else          return STOP;
}

std::string
trk::Block::
blk_name()
{
    return blk_name_;
}
