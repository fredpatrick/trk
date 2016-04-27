
#include <iostream>
#include <unistd.h>

#include "SwitchSensor.h"


trk::SwitchSensor::
SwitchSensor( int sw_num, const SW_DIRECTION& sw_direc, int sensor_fd)
{
    sw_direc_   = sw_direc;
    sw_num_     = sw_num;
    sensor_fd_  = sensor_fd;
    std::cout << "SwitchSensor.ctor: sw_num = " << sw_num_ <<
                                  " sw_direc = " << sw_direc_ << std::endl;

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
    std::cout << "SwitchSensor.event: sw_num = " << sw_num_ <<
                                   " sw_direc = " << sw_direc_ << std::endl;
    std::cout << "SwitchSensor.event: value = " << value_ << 
            " count = " << count_ << 
           ", time = " << time_of_day_.tv_sec << "." << time_of_day_.tv_usec << endl;

    string tag = "SW ";
    int ns = write(sensor_fd_, tag.c_str(), tag.length() + 1 );
    std::cout << "SwitchSensor: tag = " << tag << " written to sensor_fd" << std::endl;
    ns = write(sensor_fd_, &sw_num_, sizeof(int) );
    std::cout << "SwitchSensor: sw_num = " << sw_num_ << " written to sensor_fd" << std::endl;
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
