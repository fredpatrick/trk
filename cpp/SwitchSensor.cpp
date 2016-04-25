
#include <iostream>
#include <unistd.h>

#include "SwitchSensor.h"
#include "Switch.h"


trk::SwitchSensor::
SwitchSensor(Switch* sw, const SW_DIRECTION& sw_direc, int sensor_fd)
{
    switch_     = sw;
    sw_direc_   = sw_direc;
    sensor_fd_  = sensor_fd;
    std::cout << "SwitchSensor.ctor: sw_num = " << switch_->sw_num();
    std::cout << " sw_direc = " << sw_direc_ << std::endl;

}

trk::SwitchSensor::
~SwitchSensor()
{
    std::cout << "SwitchSensor.dtor" << std::endl;
}

void
trk::SwitchSensor::
event(int ierr, InputGPIO* gpio)
{
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    gettimeofday(&time_of_day_, NULL);
    std::cout << "SwitchSensor.event: sw_num = " << switch_->sw_num();
    std::cout << " sw_direc = " << sw_direc_ << std::endl;
    std::cout << "SwitchSensor.event: value = " << value_ << endl;

    int sw_num = switch_->sw_num();
    int ier = write(sensor_fd_, &sw_num, sizeof(int) );
    std::cout << "SwitchSensor: ier = " << ier << std::endl;
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

timeval
trk::SwitchSensor::
timeofday()
{
    return time_of_day_;
}
