
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
    
    read(sensor_fd, &nc, sizeof( int) );
    char* bs = new char[nc];
    read(sensor_fd, bs, nc);
    block_status_ = bs;
    delete bs;
}

trk::BlockEvent::
BlockEvent(double          tm_event,
            const std::string& block_name,
            const std::string& block_status)
{
    tag_          = "BLK";
    tm_event_     = tm_event;
    block_name_   = block_name;
    block_status_ = block_status;
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
    nc = block_status_.length() + 1;
    ns = write(fd, &nc, sizeof(int) );
    ns = write(fd, block_status_.c_str(), nc);
}

void
trk::BlockEvent::
print(int ntab)
{
    std::cout.width(ntab);
    std::cout << "| ";
    std::cout << "BlockEvent::" << block_name_ << " - " << 
                                 block_status_ <<  " - " << 
                                 event_seq_n_ << " - " << tm_event_ << std::endl;
}

std::string
trk::BlockEvent::
block_name()
{
    return block_name_;
}

std::string
trk::BlockEvent::
block_status()
{
    return block_status_;
}
