
#include <iostream>

#include "trkutl.h"
#include "JobClock.h"
#include "Switches.h"


using namespace trk;

int main()
{
    std::cout << "trkSwitcher: Manually set switches for trkDriver";
    std::cout << "trkSwitcher: Assumes trkDriver is running and " <<
                   "has started BBB circuits" << std::endl;

    Switches* switches =  new Switches();
    switches->manual_set();

    SW_DIRECTION sw_state[6];
    TrackState current_state = LowerLoopCW;
    TrackState next_state;
    bool done = false;
    while ( !done ) {
        std::cout << "trkSwitcher: current_state = " << current_state << std::endl;
        switches->scan(sw_state);
        for (int i = 0; i < 6; i++) std::cout << sw_state[i] << " ";
        std::cout << std::endl;
        SWKey swkey = get_switch();
        if ( swkey.num == -1 ) {
            done = true;
            break;
        } else {
            next_state = current_state;
            if ( current_state == LowerLoopCW ) {
                if (swkey.num == 1 && swkey.swd == OUT) next_state = LowerLoopCCW;
                else if (swkey.num == 2 && swkey.swd == OUT) next_state = LowerLoopCCW;
            } else if ( current_state ==  LowerLoopCCW ) {
                if ( swkey.num == 0 && swkey.swd == OUT) next_state = UpperLoopCCW;
            } else if ( current_state == UpperLoopCCW ) {
                if ( swkey.num == 4 && swkey.swd == OUT ) next_state = UpperLoopCW;
                else if ( swkey.num == 5 && swkey.swd == OUT ) next_state = UpperLoopCW;
            } else if ( current_state == UpperLoopCW ) {
                if ( swkey.num == 3 && swkey.swd == OUT) next_state = LowerLoopCW;
            }
            if (next_state != current_state ) {
                switches->set_direction(swkey);
                current_state = next_state;
            }
        }
    }
    return 0;
}


