
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
        blocks_[i] = new Block(blk_names_[i]);
        block_indexes_[blk_names_[i]] = i;
    }

    int blk_base = gpio_config->blk_base_addr();
    DemuxAddress* demux_address = DemuxAddress::instance();
    demux_address->set(blk_base);
    for ( int i = 0; i < blocks_.size(); i++) {
        blocks_[ i ]->clear();
    }
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
enable_sensors(int sensor_fd, int& n_event)
{
    for ( int i = 0; i < blocks_.size(); i++) {
        blocks_[ i ]->enable_sensor(sensor_fd, n_event);
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
