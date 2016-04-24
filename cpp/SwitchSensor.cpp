
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
}

trk::SwitchSensor::
~SwitchSensor()
{}

void
trk::SwitchSensor::
event(int ierr, InputGPIO* gpio)
{
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    gettimeofday(&time_of_day_, NULL);

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
