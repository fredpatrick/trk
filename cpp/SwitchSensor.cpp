
#include <iostream>
#include <unistd.h>

#include "SwitchSensor.h"
#include "SwitchEvent.h"
#include "JobClock.h"


trk::SwitchSensor::
SwitchSensor( int sw_num, const SW_DIRECTION& sw_direc, 
                 int sensor_fd, int& n_event) : n_event_(n_event)
{
    job_clock_  = JobClock::instance();
    sw_direc_   = sw_direc;
    sw_num_     = sw_num;
    sensor_fd_  = sensor_fd;
    ignore_event_ = false;
//  std::cout << "SwitchSensor.ctor: sw_num = " << sw_num_ <<
//                                " sw_direc = " << sw_direc_ << std::endl;

}

trk::SwitchSensor::
~SwitchSensor()
{
//  std::cout << "SwitchSensor.dtor" << std::endl;
}

void
trk::SwitchSensor::
ignore_event(bool s)
{
    ignore_event_ = s;
}

void
trk::SwitchSensor::
event(int ierr, InputGPIO* gpio)
{
    if ( ignore_event_ ) return;
    n_event_++;

    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();

    std::cout.width(50);
    std::cout << "* ";
    std::cout << "SwitchSensor.event: value = " << value_ << 
            " count = " << count_ << std::endl;

    SwitchEvent* sw_event = new SwitchEvent(tm_event_, sw_num_, sw_direc_);
    sw_event-> write_event(sensor_fd_);
    sw_event->print(50);
}

int
trk::SwitchSensor::
value()
{
    return value_;
}

int
trk::SwitchSensor::
count()
{
    return count_;
}

double
trk::SwitchSensor::
timeofday()
{
    return tm_event_;
}
