
#include <iostream>
#include <unistd.h>
#include "trkutl.h"
#include "GPIO.h"
#include "JobClock.h"
#include "Switches.h"
#include "DemuxAddress.h"
#include "EnablePCB.h"
#include "EnableBreakEvent.h"
#include "EventFactory.h"
#include "InputEvent.h"
#include "Zones.h"
#include "Blocks.h"

using namespace trk;

int main() {

    std::cout << "BeagleBoneBlack driver for exercising trk cicuits" <<std::endl;

    JobClock* job_clock = trk::JobClock::instance();
    std::cout << *job_clock << std::endl;

    EnablePCB* pcp = EnablePCB::instance();
    std::cout << "Power controler created " << std::endl;

    std::string yesno = get_yesno ( "trkDriver:Turn power on?" );
    if ( yesno == "no" ) {
        return 0;
    } else if ( yesno == "yes" ) {
         pcp->on();
    }

    SW_DIRECTION sw_state[6];
    Switches switches;
    std::cout << switches << std::endl;
    std::cout << "trkDriver: Set initial switch positions" << std::endl;
    bool done = false;
    while ( !done) {
        switches.scan(sw_state);
        for (int i = 0; i < 6; i++) std::cout << sw_state[i] << " ";
        std::cout << std::endl;
        done = switches.manual_set();
    }
    std::cout << switches << std::endl;

    int sensor_fds[2];
    if ( pipe( sensor_fds) == -1 ) {
//  if ( pipe2( sensor_fds, O_NONBLOCK) == -1 ) {
        perror("pipe");
        return false;
    }
    std::cout << "trkDriver: got the pipe" << std::endl;
    
    int n_event = 0;            // when sensors write to sensor_fds[1], n_event is
                                // incremented, when code trkDriver readfs from sensor_fds[0]
                                // n_event is decremented until pipe is empty.

    EnableBreakEvent* brk_event = new EnableBreakEvent(sensor_fds[1], n_event );
    switches.enable_sensors(sensor_fds[1], n_event );
    std::cout << "trkDriver: Switch sensors created and activated" << std::endl;

    Zones zones;
    std::cout << "trkDriver: Zone instances created" << std::endl;

    zones.enable_sensors(sensor_fds[1], n_event);
    std::cout << "trkDriver: Track  sensors created and activated" ;
    std::cout << zones << std::endl;
    Blocks blocks;
    std::cout << "trkDriver: Block instances created";
    std::cout << blocks << std::endl;

    blocks.enable_sensors(sensor_fds[1], n_event);
    std::cout << "trkDriver: Block sensors created and activated" << std::endl;

    EventFactory* event_factory = EventFactory::instance();
    InputEvent*   event;
    done = false;
    std::cout << "trkDriver: Entering event loop" << std::endl;
    while ( !done ) {
        InputEvent* event = event_factory->get_next_event(sensor_fds[0]);
        while  ( n_event > 0 ) {
            std::cout << switches << zones << blocks << std::endl;
            if ( event == 0 ) {
                std::cout << "trkDriver: Error getting next event" << std::endl;
                done = false;
                break;
            } else {
                std::cout << "| trkDriver: " << event->tag() << 
                                " received at " << *job_clock << std::endl;
                if ( event->tag() == "BRK" ) {
                    done = true;
                }
                event->print(0);
                n_event--;
                if ( n_event > 0 ) {
                    event = event_factory->get_next_event( sensor_fds[0] );
                }
            }
        }
    }

    delete brk_event;
//  delete switches;
    pcp->off();
    delete pcp;
    DemuxAddress* da = DemuxAddress::instance();
    delete da;
    return 0;
}
