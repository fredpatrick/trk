
#include <iostream>
#include <unistd.h>

#include "TrackSensor.h"
#include "JobClock.h"

trk::TrackSensor::
TrackSensor( int                 sensor_fd, 
                   int&               n_event, 
                   const std::string& zone_name) : n_event_(n_event)
{
    job_clock_  = JobClock::instance();
    sensor_fd_  = sensor_fd;
    ignore_event_ = false;
    zone_name_  = zone_name;
//  std::cout << "TrackSensor.ctor:" ;

}

trk::TrackSensor::
~TrackSensor()
{
//  std::cout << "TrackSensor.dtor" << std::endl;
}

void
trk::TrackSensor::
ignore_event(bool s)
{
    ignore_event_ = true;
}

void
trk::TrackSensor::
event(int ierr, InputGPIO* gpio)
{
    if ( ignore_event_ ) return;
    n_event_++;
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();
    std::cout.width(50);
    std::cout << "* ";
    std::cout << "TrackSensor.event. n_event = " << n_event_ << " - " << tm_event_ << std::endl;
    std::cout.width(50);
    std::cout << "* " << "TrackSensor.event, " << zone_name_ << 
                                  ", value = " << value_ << std::endl; 
    string tag = "TRK";
    int ns = write(sensor_fd_, tag.c_str(), tag.length() + 1 );
    int nc = zone_name_.length() + 1;
    ns = write(sensor_fd_, &nc, sizeof(int) );
    ns = write(sensor_fd_, zone_name_.c_str(), nc);
    ns = write(sensor_fd_, &value_, sizeof(int) );
}

int
trk::TrackSensor::
value()
{
    return value_;
}

int
trk::TrackSensor::
count()
{
    return count_;
}

double
trk::TrackSensor::
timeofday()
{
    return tm_event_;
}
