
#include "BreakEvent.h"

#include <iostream>
#include <unistd.h>


trk::BreakEvent::
BreakEvent(int sensor_fd)
{
    tag_ = "BRK";
    read(sensor_fd, &tm_event_, sizeof(double) );
}

trk::BreakEvent::
BreakEvent(double      tm_event)
{
    tag_        = "BRK";
    tm_event_   = tm_event;
}


int
trk::BreakEvent::
write_event(int fd)
{
    int ns = write(fd, tag_.c_str(), tag_.length() + 1 );
    ns = write(fd, &tm_event_, sizeof(double) );

}

void
trk::BreakEvent::
print(int ntab)
{
    std::cout.width(ntab);
    std::cout << "| ";
    std::cout << "BreakEvent::" << tm_event_ << std::endl;
}
