
#include <iostream>

#include "GPIOConfig.h"
#include "Zones.h"
#include "Zone.h"

trk::Zones::
Zones()
{
    gpio_config_ = GPIOConfig::instance();
    zone_names_ = gpio_config_->zone_names();
    for ( int i = 0; i < zone_names_.size(); i++) {
        zones_[zone_names_[i] ] = new Zone(zone_names_[i]);
    }
}

trk::Zones::
~Zones() {
    for ( int i = 0; i < zone_names_.size(); i++) {
        delete zones_[zone_names_[i] ];
    }
}


bool
trk::Zones::
enable_sensors(int sensor_fd, int& n_event)
{
    for ( int i = 0; i < 6; i++) {
        zones_[ zone_names_[i] ]->enable_sensor(sensor_fd, n_event);
    }
    return true;
}

bool
trk::Zones::
blockit(const std::string& zone_name)
{
    return zones_[ zone_name]->blockit();
}
