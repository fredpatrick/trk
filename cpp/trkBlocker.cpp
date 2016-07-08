
#include <iostream>

#include "JobClock.h"
#include "Blocks.h"

using namespace trk;

int main()
{
    std::cout << "trkBlock: Manually set blocks" << std::endl;
    std::cout << "trkBlock: Assumes trkDriver is running and " <<
                   "has started BBB circuits" << std::endl;

    Blocks* blocks =  new Blocks();

    std::string blk_name;
    bool done = false;
    while (!done ) {
        for ( int i = 0; i < blocks->n_block(); i++) {
            std:: cout << "trkBlocker: " << blocks->blk_name(i) << 
                                   " = " << blocks->get_state(i);
        }
        std::cout << std::endl;
        std::cout << "trkBlocker: Enter Block name (O72B|O60B|Done):";
        std::cin >> blk_name;
        if ( blk_name == "Done") {
            done = true;
        } else {
            bool ret = blocks->blockit(blk_name);
            if ( !ret ) {
                std::cout << "trkBlock: Couldn't set Block" << std::endl;
            }
        }
    }

    return 0;
}


