
#include <iostream>
#include <unistd.h>

#include "BlockSensor.h"
#include "BlockEvent.h"
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
    int ier = pthread_mutex_lock(&write_event_);
    if ( ier != 0 ) std::cout << "BlockSensor.event, couldn't lock mutex, ier = " <<
                                ier << std::endl;
    if ( ignore_event_ ) return;
    n_event_++;
    value_ = (int)gpio->value();
    std::string blk_status;
    if      ( value_ == 0 ) blk_status = "ON";
    else if ( value_ == 1 ) blk_status = "OFF";
    count_ = gpio->ev_count();
    tm_event_ = job_clock_->job_time();
    std::cout.width(50);
    std::cout << "| ";
    std::cout << "BlockSensor.event. n_event = " << n_event_ << " - " << tm_event_ << std::endl;

    BlockEvent* blk_event = new BlockEvent(tm_event_, blk_name_, blk_status);
    blk_event->write_event(sensor_fd_);
    blk_event->print(50);
    ier = pthread_mutex_unlock(&write_event_);
    if ( ier != 0 ) std::cout << "BlockSensor.event, couldn't unlock mutex, ier = " <<
                                ier << std::endl;
    string tag = "TRK";
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
