
#include "BreakEvent.h"

#include <iostream>
#include <unistd.h>


trk::BreakEvent::
BreakEvent(int sensor_fd)
{
    tag_ = "BRK";
    read(sensor_fd, &tm_event_, sizeof(double) );
    read(sensor_fd, &event_seq_n_, sizeof(int) );
}

trk::BreakEvent::
BreakEvent(double      tm_event)
{
    tag_        = "BRK";
    tm_event_   = tm_event;
    event_seq_n_++;
}


int
trk::BreakEvent::
write_event(int fd)
{
    int ns = write(fd, tag_.c_str(), tag_.length() + 1 );
    ns = write(fd, &tm_event_, sizeof(double) );
    ns = write(fd, &event_seq_n_, sizeof(int) );
}

void
trk::BreakEvent::
print(int ntab)
{
    std::cout.width(ntab);
    std::cout << "| ";
    std::cout << "BreakEvent::" << event_seq_n_ << " - " << tm_event_ << std::endl;
}
