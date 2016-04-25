
#include <iostream>
#include <unistd.h>
#include "GPIO.h"
#include "DemuxAddress.h"
#include "Switches.h"
#include "EnablePCB.h"

using namespace trk;

int main() {

    std::cout << "BeagleBoneBlack driver for exercising trk cicuits" <<std::endl;

    EnablePCB* pcp = EnablePCB::instance();
    std::cout << "Power controler created " << std::endl;

    int sensor_fds[2];
    if ( pipe( sensor_fds) == -1 ) {
        perror("pipe");
        return false;
    }
    std::cout << "trkDriver: got the pipe" << std::endl;

    Switches* switches = new Switches(sensor_fds[1]);
    std::cout << "trkDriver: Switch sensors created and activated" << endl;

    DemuxAddress* dmxaddr = DemuxAddress::instance();
    std::cout << "trkDriver: demoxaddress gpios created" << std::endl;

//  usleep(100000000);
//  cout << "trkDriver, finished sleeping for 100 seconds" << endl;

    std::string yesno = "no";
    while ( yesno == "no" ) {
        std::cout << "Turn power on? (yes/no) :";
        std::cin >> yesno;
        if ( yesno == "no" ) return 0;
    }
    pcp->on();

    bool done = false;
    while ( !done ) {
        std::cout << "trkDriver: Enter switch index (-1 / 0:5) and switch direction " <<
                        "(THRU/OUT): ";
        int          switch_index;
        SW_DIRECTION sw_direc;
        std::cin >> switch_index;
        if ( switch_index == -1 ) {
            done = true;
        } else {
            cin >> sw_direc;
            dmxaddr->set(switch_index, sw_direc);
        }
    }

    pcp->off();
    delete switches;
    delete dmxaddr;
    delete pcp;
    return 0;
}

