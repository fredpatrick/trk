
#include "SwitchEvent.h"
#include <iostream>
#include <unistd.h>

trk::SwitchEvent::
SwitchEvent(int sensor_fd)
{
    tag_ = "SW ";

    read(sensor_fd, &tm_event_, sizeof(double) );
    read(sensor_fd, &event_seq_n_, sizeof(int) );
    read(sensor_fd, &sw_num_, sizeof(int));
    read(sensor_fd, &sw_direc_, sizeof(int));
}

trk::SwitchEvent::
SwitchEvent(double          tm_event,
            int             sw_num,
            SW_DIRECTION    sw_direc)
{
    tag_          = "SW ";
    tm_event_     = tm_event;
    sw_num_       = sw_num;
    sw_direc_     = sw_direc;
    event_seq_n_++;
}

int
trk::SwitchEvent::
write_event(int fd)
{
    int ns = write(fd, tag_.c_str(), tag_.length() + 1 );
    ns = write(fd, &tm_event_, sizeof(double) );
    ns = write(fd, &event_seq_n_, sizeof(int) );
    ns = write(fd, &sw_num_,   sizeof(int) );
    ns = write(fd, &sw_direc_, sizeof(int) );
}

void
trk::SwitchEvent::
print(int ntab)
{
    std::cout.width(ntab);
    std::cout << "| ";
    std::cout << "SwitchEvent::" << sw_num_ << " - " << 
                                 sw_direc_ <<  " - " << " - " <<
                                 event_seq_n_ << " - " << tm_event_ << std::endl;
}

int
trk::SwitchEvent::
sw_num()
{
    return sw_num_;
}

trk::SW_DIRECTION
trk::SwitchEvent::
sw_direc()
{
    return sw_direc_;
}
