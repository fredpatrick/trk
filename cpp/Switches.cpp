#include "Switches.h"
#include <iostream>

trk::Switches::
Switches()
{
    for ( int i = 0; i < 6; i++) {
        switch_[i] = new Switch(i);
    }
}

trk::Switches::
~Switches()
{
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
