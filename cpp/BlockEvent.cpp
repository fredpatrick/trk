
#include "BlockEvent.h"
#include <iostream>
#include <unistd.h>

trk::BlockEvent::
BlockEvent(int sensor_fd)
{
    tag_ = "BLK";

    read(sensor_fd, &tm_event_, sizeof(double) );
    read(sensor_fd, &event_seq_n_, sizeof(int) );
    int nc;
    read(sensor_fd, &nc, sizeof(int) );
    char* bn = new char[nc];
    read(sensor_fd, bn, nc);
    block_name_ = bn;
    delete bn;
    
    read(sensor_fd, &block_state_, sizeof(int) );
}

trk::BlockEvent::
BlockEvent(double          tm_event,
            const std::string& block_name,
            const BLK_STATE&   block_state)
{
    tag_          = "BLK";
    tm_event_     = tm_event;
    block_name_   = block_name;
    block_state_  = block_state;
    event_seq_n_++;
}

int
trk::BlockEvent::
write_event(int fd)
{
    int ns = write(fd, tag_.c_str(), tag_.length() + 1 );
    ns = write(fd, &tm_event_, sizeof(double) );
    ns = write(fd, &event_seq_n_, sizeof(int) );
    int nc = block_name_.length() + 1;
    ns = write(fd, &nc, sizeof(int) );
    ns = write(fd, block_name_.c_str(), nc);
    ns = write(fd, &block_state_, sizeof(int) );
}

void
trk::BlockEvent::
print(int ntab)
{
    std::cout.width(ntab);
    std::cout << "| ";
    std::cout << "BlockEvent::" << block_name_ << " - " << 
                                 block_state_ <<  " - " << 
                                 event_seq_n_ << " - " << tm_event_ << std::endl;
}

std::string
trk::BlockEvent::
block_name()
{
    return block_name_;
}

trk::BLK_STATE
trk::BlockEvent::
block_state()
{
    return block_state_;
}
