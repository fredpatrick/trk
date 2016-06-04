
#include <iostream>
#include <unistd.h>

#include "BreakSensor.h"
#include "BreakEvent.h"
#include "JobClock.h"

trk::BreakSensor::
BreakSensor( int sensor_fd, int& n_event) : n_event_(n_event)
{
    job_clock_  = JobClock::instance();
    sensor_fd_  = sensor_fd;

}

trk::BreakSensor::
~BreakSensor()
{
//  std::cout << "BreakSensor.dtor" << std::endl;
}

void
trk::BreakSensor::
event(int ierr, InputGPIO* gpio)
{
    n_event_++;
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();
    std::cout.width(50);
    std::cout << "| ";
    std::cout << "BreakSensor.event. n_event = " << n_event_ << " - " << tm_event_ << std::endl;
    BreakEvent* brk_event = new BreakEvent(tm_event_);
    brk_event->write_event(sensor_fd_);
    brk_event->print(50);
}

int
trk::BreakSensor::
value()
{
    return value_;
}

int
trk::BreakSensor::
count()
{
    return count_;
}

double
trk::BreakSensor::
timeofday()
{
    return tm_event_;
}
