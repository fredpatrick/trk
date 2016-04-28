
#include <iostream>
#include <unistd.h>

#include "BrkEventSensor.h"
#include "JobClock.h"

trk::BrkEventSensor::
BrkEventSensor( int sensor_fd)
{
    job_clock_  = JobClock::instance();
    sensor_fd_  = sensor_fd;
//  std::cout << "BrkEventSensor.ctor:" ;

}

trk::BrkEventSensor::
~BrkEventSensor()
{
//  std::cout << "BrkEventSensor.dtor" << std::endl;
}

void
trk::BrkEventSensor::
event(int ierr, InputGPIO* gpio)
{
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();
    std::cout.width(40);
    std::cout << "* ";
    std::cout << "BrkEventSensor.event:" << " - " << tm_event_ << std::endl;
    std::cout.width(40);
    std::cout << "* ";
    std::cout << "BrkEventSensor.event: value = " << value_ << 
            " count = " << count_ << std::endl;

    string tag = "BRK";
    int ns = write(sensor_fd_, tag.c_str(), tag.length() + 1 );
}

int
trk::BrkEventSensor::
value()
{
    return value_;
}

int
trk::BrkEventSensor::
count()
{
    return count_;
}

double
trk::BrkEventSensor::
timeofday()
{
    return tm_event_;
}
