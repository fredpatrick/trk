#include "Switches.h"
#include <iostream>

trk::Switches::
Switches(int sensor_fd)
{
    std::cout << "Switches:ctor" << endl;
    for ( int i = 0; i < 6; i++) {
        switch_[i] = new Switch(i, sensor_fd);
    }
}

trk::Switches::
~Switches()
{
    std::cout << "Switches:dtor" << endl;
    delete switch_;
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
