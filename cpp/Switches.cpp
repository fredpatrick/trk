
#include <iostream>
#include "Switches.h"

trk::Switches::
Switches()
{
//  std::cout << "Switches:ctor" << std::endl;
    for ( int i = 0; i < 6; i++) {
        switch_[i] = new Switch(i);
        switch_[i]->set_direction(THRU);
    }
}

trk::Switches::
~Switches()
{
//  std::cout << "Switches:dtor" << std::endl;
    for ( int i = 0; i < 6; i++) {
        delete switch_[i];
    }
}

bool
trk::Switches::
enable_sensors(int sensor_fd, int& n_event)
{
    for ( int i = 0; i < 6; i++) {
        switch_[i]->enable_sensors(sensor_fd, n_event);
    }
    return true;
}

void
trk::Switches::
set_direction( const SWKey& key)
{
    switch_[key.num]->set_direction(key.swd);
}

void
trk::Switches::
manual_set()
{
    bool done = false;
    while ( !done  ) {
        SWKey key = get_switch();
        if ( key.num == -1 ) {
            done = true;
        } else {
            set_direction(key);
            done = false;
        }
    }
}

void
trk::Switches::
list_state()
{
    for ( int i = 0; i < 6; i++) {
        std::cout << switch_[i]->state();
    }
    std::cout << std::endl;
}
