

#include <iostream>
#include "Switches.h"
#include "trkutl.h"

trk::Switches::
Switches()
{
    std::cout << "Switches:ctor" << std::endl;
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

bool
trk::Switches::
manual_set()
{
    SW_DIRECTION sw_state[6];
    bool done = false;
    SWKey key = get_switch_key();
    if ( key.num == -1 ) {
        return true;
    } else {
        set_direction(key);
        return false;
    }
}

trk::SWKey
trk::Switches::
get_switch_key()
{
    SWKey key;
    bool got_ans = false;
    while ( !got_ans ) {
        std::cout << "get_switch:enter sw_num (-1:5), sw_direc (THRU/OUT): " << std::endl;
        std::cin >> key.num;
        if ( std::cin.fail() ) {
            std::cout << "Bad read on sw_num" << std::endl;
            std::cin.clear();
            std::cin.ignore(100, '\n');
            continue;
        }
        std::cout << key.num << std::endl;
        if ( key.num >= -1 && key.num <= 5 ) {
            if ( key.num == -1 ) {
                got_ans = true;
            } else {
                std::cin >> key.swd;
                if ( key.swd == THRU || key.swd == OUT ) {
                    std::cout << "get_switch, switch key = " << key  << std::endl;
                    got_ans = true;
                } else {
                    got_ans = false;
                }
            }
        }
    }
    return key;
}

void
trk::Switches::
scan(SW_DIRECTION* sw_state)
{
    for ( int i = 0; i < 6; i++) {
        SW_DIRECTION swd = switch_[i]->state();
        sw_state[i] = swd;
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

trk::Switch*
trk::Switches::
swtch(int i) const
{
    return switch_[i];
}

std::ostream& 
trk::operator<<( std::ostream& ostrm, const trk::Switches& switches)
{
    for ( int i = 0; i < 6; i++) {
        ostrm << switches.swtch(i)->state();
    }
    return  ostrm;
}

