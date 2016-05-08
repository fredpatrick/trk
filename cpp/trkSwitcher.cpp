
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
    return 0;
}


