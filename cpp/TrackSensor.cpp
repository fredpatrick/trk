
#include <iostream>
#include <unistd.h>

#include "TrackSensor.h"
#include "TrackEvent.h"
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
    std::string type;
    if      ( value_ == 0) type = "ENTRY";
    else if ( value_ == 1) type = "EXIT";
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();
    std::cout.width(50);
    std::cout << "* ";
    std::cout << "TrackSensor.event. n_event = " << n_event_ << " - " << tm_event_ << std::endl;
    TrackEvent* trk_event = new TrackEvent(tm_event_, zone_name_, type);
    trk_event->write_event(sensor_fd_);
    trk_event->print(50);
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
