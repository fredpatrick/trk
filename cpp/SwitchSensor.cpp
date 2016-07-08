
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
    int ier = pthread_mutex_lock(&write_event_);
    if ( ier != 0 ) std::cout << "SwitchSensor.event, couldn't lock mutex, ier = " <<
                                 ier << std::endl;
    n_event_++;

    value_ = gpio->value();
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();

    std::cout.width(50);
    std::cout << "| ";
    std::cout << "SwitchSensor.event: value = " << value_ << 
            " count = " << count_ << std::endl;

    SwitchEvent* sw_event = new SwitchEvent(tm_event_, sw_num_, sw_direc_);
    sw_event-> write_event(sensor_fd_);
    sw_event->print(50);
    ier = pthread_mutex_unlock(&write_event_);
    if ( ier != 0 ) std::cout << "SwitchSensor.event, couldn't unlock mutex, ier = " <<
                                 ier << std::endl;
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
