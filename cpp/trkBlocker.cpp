
#include <iostream>

#include "JobClock.h"
#include "Blocks.h"

using namespace trk;

int main()
{
    std::cout << "trkBlock: Manually set blocks" << std::endl;
    std::cout << "trkBlock: Assumes trkDriver is running and " <<
                   "has started BBB circuits" << std::endl;

    Blocks* blockers =  new Blocks();

    std::string blk_name;
    bool done = false;
    while (!done ) {
        std::cout << "trkBlock: Enter Block name (O72B|O60B|Done):";
        std::cin >> blk_name;
        if ( blk_name == "Done") {
            done = true;
        } else {
            bool ret = blockers->blockit(blk_name);
            if ( !ret ) {
                std::cout << "trkBlock: Couldn't set Block" << std::endl;
            }
        }
    }

    return 0;
}


