#include "Zone.h"
#include "TrackSensor.h"
#include "GPIO.h"
#include "GPIOConfig.h"
#include <iostream>


trk::
Zone::Zone(const std::string& zone_name)
{
    track_sensor_ = 0;

//  std::cout << "Zone.ctor: zone_name = " << zone_name << std::endl;
    zone_name_ = zone_name;
    GPIOConfig* gpio_config = GPIOConfig::instance();
    track_gpio_  = gpio_config->track_gpio(zone_name_);
    track_sensor_ = 0;
//  std:cout << "Zone.ctor: Got the gpios" << endl;
}

trk::Zone::
~Zone()
{
//  std::cout << "Zone.dtor" << std::endl;
    if ( track_sensor_) track_sensor_->ignore_event(true);
    delete track_gpio_;
}

bool
trk::Zone::
enable_sensor(int sensor_fd, int& n_event)
{

    track_sensor_= new TrackSensor(sensor_fd, n_event, zone_name_);
    track_gpio_->edge_type(BOTH);
    track_gpio_->debounce_time(200);
    track_gpio_->wait_for_edge(track_sensor_);
    std::cout << "Zone.ctor, Poll started on Zone " << zone_name_ << endl;

    return true;
}

bool
trk::Zone::
state()
{
    int v = track_gpio_->value();
    return v;
}

std::string
trk::Zone::
zone_name()
{
    return zone_name_;
}
