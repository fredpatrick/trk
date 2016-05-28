
#include <iostream>
#include <unistd.h>

#include "BlockSensor.h"
#include "JobClock.h"

trk::BlockSensor::
BlockSensor( int                 sensor_fd, 
                   int&               n_event, 
                   const std::string& blk_name) : n_event_(n_event)
{
    job_clock_  = JobClock::instance();
    sensor_fd_  = sensor_fd;
    ignore_event_ = false;
    blk_name_  = blk_name;
//  std::cout << "BlockSensor.ctor:" ;

}

trk::BlockSensor::
~BlockSensor()
{
//  std::cout << "BlockSensor.dtor" << std::endl;
}

void
trk::BlockSensor::
ignore_event()
{
    ignore_event_ = true;
}

void
trk::BlockSensor::
event(int ierr, InputGPIO* gpio)
{
    if ( ignore_event_ ) return;
    n_event_++;
    value_ = (int)gpio->value();
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();
    std::cout.width(50);
    std::cout << "* ";
    std::cout << "BlockSensor.event. n_event = " << n_event_ << " - " << tm_event_ << std::endl;
    std::cout.width(50);
    std::cout << "* " << "BlockSensor.event, " << blk_name_ << 
                                  ", value = " << value_ << std::endl; 
    string tag = "TRK";
    int ns = write(sensor_fd_, tag.c_str(), tag.length() + 1 );
    int nc = blk_name_.length() + 1;
    ns = write(sensor_fd_, &nc, sizeof(int) );
    ns = write(sensor_fd_, blk_name_.c_str(), nc);
    ns = write(sensor_fd_, &value_, sizeof(int) );
}

int
trk::BlockSensor::
value()
{
    return value_;
}

int
trk::BlockSensor::
count()
{
    return count_;
}

double
trk::BlockSensor::
timeofday()
{
    return tm_event_;
}
