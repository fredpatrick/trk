#include "Switches.h"
#include <iostream>

trk::Switches::
Switches()
{
    std::cout << "Switches:ctor" << endl;
    for ( int i = 0; i < 6; i++) {
        switch_[i] = new Switch(i);
        switch_[i]->set_direction(THRU);
    }
}

trk::Switches::
~Switches()
{
    std::cout << "Switches:dtor" << endl;
    for ( int i = 0; i < 6; i++) {
        delete switch_[i];
    }
}

bool
trk::Switches::
enable_sensors(int sensor_fd)
{
    for ( int i = 0; i < 6; i++) {
        switch_[i]->enable_sensors(sensor_fd);
    }
    return true;
}

void
trk::Switches::
set_direction(int sw_num, const SW_DIRECTION& sw_direc)
{
    switch_[sw_num]->set_direction(sw_direc);
}

void
trk::Switches::
manual_set()
{
    bool done = false;
    while ( !done  ) {
        std::cout << "trkDriver: Enter switch index (-1 || 0:5) and direction " <<
                    "(THRU/OUT): ";
        int          sw_num;
        SW_DIRECTION sw_direc;
        std::cin >> sw_num;
        if ( sw_num == -1 ) {
            done = true;
        } else {
            cin >> sw_direc;
            set_direction(sw_num, sw_direc);
            done = false;
        }
    }
}

void
trk::Switches::
list_state()
{
    for ( int i = 0; i < 6; i++) {
        cout << switch_[i]->state();
    }
    cout << endl;
}
