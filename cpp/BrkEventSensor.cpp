
#include <iostream>
#include <unistd.h>

#include "BrkEventSensor.h"

trk::BrkEventSensor::
BrkEventSensor( int sensor_fd)
{
    sensor_fd_  = sensor_fd;
    std::cout << "BrkEventSensor.ctor:" ;

}

trk::BrkEventSensor::
~BrkEventSensor()
{
    std::cout << "BrkEventSensor.dtor" << std::endl;
}

void
trk::BrkEventSensor::
event(int ierr, InputGPIO* gpio)
{
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    gettimeofday(&time_of_day_, NULL);
    std::cout << "BrkEventSensor.event: value = " << value_ <<
            " count = " << count_ << 
           ", time = " << time_of_day_.tv_sec << "." << time_of_day_.tv_usec << endl;

    string tag = "BRK";
    int ier = write(sensor_fd_, tag.c_str(), tag.length() + 1 );
    std::cout << "BrkEventSensor: tag = " << tag << " written to sensor_fd" << std::endl;
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

timeval
trk::BrkEventSensor::
timeofday()
{
    return time_of_day_;
}
