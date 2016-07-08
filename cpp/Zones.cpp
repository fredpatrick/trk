
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
        Zone* z  = new Zone(zone_names_[i]);
        zones_.push_back(z);
        zone_indexes_[zone_names_[i]] = i;
    }
}

trk::Zones::
~Zones() {
    for ( int i = 0; i < zone_names_.size(); i++) {
        delete zones_[i];
    }
}


bool
trk::Zones::
enable_sensors(int sensor_fd, int& n_event)
{
    for ( int i = 0; i < zones_.size(); i++) {
        zones_[ i ]->enable_sensor(sensor_fd, n_event);
    }
    return true;
}

int
trk::Zones::
n_zone() const
{
    return zones_.size();
}

trk::TRK_STATE
trk::Zones::
get_state(int i) const
{
    return zones_[i]->state();
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::Zones& zones)
{
    int n = zones.n_zone();
    for ( int i = 0; i < n; i++) ostrm << zones.get_state(i);
    return ostrm;
}
