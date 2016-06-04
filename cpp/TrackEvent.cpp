
#include <iostream>
#include <unistd.h>

#include "TrackEvent.h"

trk::TrackEvent::
TrackEvent(int sensor_fd)
{
    tag_ = "TRK";
    read(sensor_fd, &tm_event_, sizeof(double) );
    int nc;
    read(sensor_fd, &nc, sizeof(int) );
    char* zn = new char[nc];
    read(sensor_fd, zn, nc);
    zone_name_ = zn;
    delete zn;
    int v;
    read(sensor_fd, &nc, sizeof(int) );
    char* ty = new char[nc];
    read(sensor_fd, ty, sizeof(int) );
    type_ = ty;
}

trk::TrackEvent::
TrackEvent(double          tm_event,
            const std::string& zone_name,
            const std::string& type)
{
    tag_       = "TRK";
    tm_event_  = tm_event;
    zone_name_ = zone_name;
    type_      = type;
}

int
trk::TrackEvent::
write_event(int fd)
{

    int ns = write(fd, tag_.c_str(), tag_.length() + 1 );
    int nc = zone_name_.length() + 1;
    ns = write(fd, &nc, sizeof(int) );
    ns = write(fd, zone_name_.c_str(), nc);
    nc = type_.length() + 1;
    ns = write(fd, &nc, sizeof(int) );
    ns = write(fd, type_.c_str(), nc);
}

void
trk::TrackEvent::
print(int ntab)
{
    std::cout.width(ntab);
    std::cout << "| ";
    std::cout << "TrackEvent::" << zone_name_ << " - " << 
                                 type_ <<  " - " << tm_event_ << std::endl;
}

std::string
trk::TrackEvent::
zone_name()
{
    return zone_name_;
}

std::string
trk::TrackEvent::
type()
{
    return type_;
}
