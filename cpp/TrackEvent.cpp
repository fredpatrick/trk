
#include <iostream>
#include <unistd.h>

#include "TrackEvent.h"

trk::TrackEvent::
TrackEvent(int sensor_fd)
{
    tag_ = "TRK";
    int ier =    read(sensor_fd, &tm_event_, sizeof(double) );
    ier =    read(sensor_fd, &event_seq_n_, sizeof(int) );
    int nc;
    ier = read(sensor_fd, &nc, sizeof(int) );
    char* zn = new char[nc];
    ier = read(sensor_fd, zn, nc);
    zone_name_ = zn;
    delete zn;
    ier = read(sensor_fd, &track_state_, sizeof(int) );
}

trk::TrackEvent::
TrackEvent(double          tm_event,
            const std::string& zone_name,
            const TRK_STATE&   track_state)
{
    tag_         = "TRK";
    tm_event_    = tm_event;
    zone_name_   = zone_name;
    track_state_ = track_state;
    event_seq_n_++;
}

int
trk::TrackEvent::
write_event(int fd)
{

    int ns = write(fd, tag_.c_str(), tag_.length() + 1 );
    ns = write(fd, &tm_event_, sizeof(double) );
    ns = write(fd, &event_seq_n_, sizeof(int) );
    int nc = zone_name_.length() + 1;
    ns = write(fd, &nc, sizeof(int) );
    ns = write(fd, zone_name_.c_str(), nc);
    ns = write(fd, &track_state_, sizeof(int));
}

void
trk::TrackEvent::
print(int ntab)
{
    std::cout.width(ntab);
    std::cout << "| ";
    std::cout << "TrackEvent::" << zone_name_ << " - " << 
                          track_state_ <<  " - " << event_seq_n_ << " - "<< tm_event_ << std::endl;
}

std::string
trk::TrackEvent::
zone_name()
{
    return zone_name_;
}

trk::TRK_STATE
trk::TrackEvent::
track_state()
{
    return track_state_;
}
