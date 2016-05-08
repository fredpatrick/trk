
#include <iostream>

#include "JobClock.h"
#include "Blockers.h"

using namespace trk;

int main()
{
    std::cout << "trkBlocker: Manually set blocks" << std::endl;
    std::cout << "trkBlocker: Assumes trkDriver is running and " <<
                   "has started BBB circuits" << std::endl;

    Blockers* blockers =  new Blockers();

    std::string blk_name;
    bool done = false;
    while (!done ) {
        std::cout << "trkBlocker: Enter Blocker name (O72B|O60B|Done):";
        std::cin >> blk_name;
        if ( blk_name == "Done") {
            done = true;
        } else {
            bool ret = blockers->blockit(blk_name);
            if ( !ret ) {
                std::cout << "trkBlocker: Couldn't set Block" << std::endl;
            }
        }
    }

    return 0;
}


