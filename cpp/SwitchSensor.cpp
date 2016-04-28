
#include <iostream>
#include <unistd.h>

#include "SwitchSensor.h"
#include "JobClock.h"


trk::SwitchSensor::
SwitchSensor( int sw_num, const SW_DIRECTION& sw_direc, int sensor_fd)
{
    job_clock_  = JobClock::instance();
    sw_direc_   = sw_direc;
    sw_num_     = sw_num;
    sensor_fd_  = sensor_fd;
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
event(int ierr, InputGPIO* gpio)
{
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();
    std::cout.width(40);
    std::cout << "* ";
    std::cout << "SwitchSensor.event:" << sw_num_ << ", " <<  sw_direc_ << 
                        " - " << tm_event_ << std::endl;
    std::cout.width(40);
    std::cout << "* ";
    std::cout << "SwitchSensor.event: value = " << value_ << 
            " count = " << count_ << std::endl;

    string tag = "SW ";
    int ns = write(sensor_fd_, tag.c_str(), tag.length() + 1 );
    ns = write(sensor_fd_, &sw_num_, sizeof(int) );
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
