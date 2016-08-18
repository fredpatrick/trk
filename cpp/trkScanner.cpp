#include <iostream>

#include "Switches.h"
#include "Zones.h"
#include "Blocks.h"

using namespace trk;

int main()
{
    std::cout << "trkScanner: Scan all sensors" << std::endl;

    Switches    switches;
    Blocks      blocks;
    Zones       zones;


    bool done = false;
    while ( !done) {
        std::cout << "trkScanner: Enter command (scan|done) :";
        std::string ans;
        std::cin >> ans;
        if ( ans == "scan" ) {
            std::cout << switches << zones << blocks << std::endl;
        } else if ( ans == "done" ) {
            done = true;
        }
    }
    return 0;
}
