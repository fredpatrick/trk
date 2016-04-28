
#include <iostream>
#include <unistd.h>
#include "GPIO.h"
#include "DemuxAddress.h"
#include "JobClock.h"
#include "Switches.h"
#include "EnablePCB.h"
#include "EnableBrkEvent.h"

using namespace trk;

int main() {

    std::cout << "BeagleBoneBlack driver for exercising trk cicuits" <<std::endl;

    JobClock* job_clock = trk::JobClock::instance();
    std::cout << *job_clock << std::endl;

    EnablePCB* pcp = EnablePCB::instance();
    std::cout << "Power controler created " << std::endl;

    int sensor_fds[2];
    if ( pipe( sensor_fds) == -1 ) {
        perror("pipe");
        return false;
    }
    std::cout << "trkDriver: got the pipe" << std::endl;
    
    EnableBrkEvent* brk_event = new EnableBrkEvent(sensor_fds[1] );

    Switches* switches = new Switches(sensor_fds[1]);
    std::cout << "trkDriver: Switch sensors created and activated" << endl;

    DemuxAddress* dmxaddr = DemuxAddress::instance();
    std::cout << "trkDriver: demoxaddress gpios created" << std::endl;

    bool done;
    std::string yesno = "";
    while ( yesno == "" ) {
        std::cout << "Turn power on? (yes/no) :";
        std::cin >> yesno;
        if ( yesno == "no" ) {
            done = true;
        } else if ( yesno == "yes" ) {
            pcp->on();
            done = false;
        }
    }

    while ( !done ) {
        std::cout << "trkDriver: Read on event pipe" << endl;
        char* ctag = new char[4];
        read(sensor_fds[0], ctag, 4); 
        std::string tag = ctag;
        std::cout << "trkDriver:  event " << tag << " - "<< *job_clock << std::endl;
        if ( tag == "BRK" ) {
            done = true;
        } else if ( tag == "SW " ) {
            int sw_num;
            read(sensor_fds[0], &sw_num, sizeof(int) );
            yesno = "";
            std::cout << "trkDriver:flip switches? (yes|no):" << std::endl;
            std::cin >> yesno;
            if ( yesno == "yes" ) {
                std::cout << "trkDriver: Enter switch index (-1 || 0:5) and direction " <<
                            "(THRU/OUT): ";
                int          switch_index;
                SW_DIRECTION sw_direc;
                std::cin >> switch_index;
                if ( switch_index == -1 ) {
                    done = true;
                } else {
                    cin >> sw_direc;
                    dmxaddr->set(switch_index, sw_direc);
                    done = false;
                }
            } else {
                done = false;
            }
        }
    }

    pcp->off();
    delete brk_event;
    delete switches;
    delete dmxaddr;
    delete pcp;
    return 0;
}

