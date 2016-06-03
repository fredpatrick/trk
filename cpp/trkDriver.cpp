
#include <iostream>
#include <unistd.h>
#include "trkutl.h"
#include "GPIO.h"
#include "JobClock.h"
#include "Switches.h"
#include "DemuxAddress.h"
#include "EnablePCB.h"
#include "EnableBrkEvent.h"
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

    Switches* switches = new Switches();
    std::cout << "trkDriver: Set initial switch positions" << std::endl;
    switches->manual_set();

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

    EnableBrkEvent* brk_event = new EnableBrkEvent(sensor_fds[1], n_event );
    switches->enable_sensors(sensor_fds[1], n_event );
    std::cout << "trkDriver: Switch sensors created and activated" << std::endl;

    Zones* zones = new Zones();
    std::cout << "trkDriver: Zone instaces created" << std::endl;

    zones->enable_sensors(sensor_fds[1], n_event);
    std::cout << "trkDriver: Track  sensors created and activated" << std::endl;

    Blocks* blocks = new Blocks();
    std::cout << "trkDriver: Block instaces created" << std::endl;

    blocks->enable_sensors(sensor_fds[1], n_event);
    std::cout << "trkDriver: Block sensors created and activated" << std::endl;

    bool done = false;
    while ( !done ) {
        std::cout << "trkDriver: Read on event pipe, n_event = " << n_event << std::endl;
        char* ctag = new char[4];
        read(sensor_fds[0], ctag, 4); 
        std::string tag = ctag;
        std::string zone_name;
        while  ( n_event > 0 ) {
            std::cout << "trkDriver: event " << tag << ", n_event=" <<
                                n_event << ", " << *job_clock << std::endl;
            zones->scan();
            if ( tag == "BRK" ) {
                done = true;
            } else if ( tag == "SW " ) {
                int sw_num;
                read(sensor_fds[0], &sw_num, sizeof(int) );
                SW_DIRECTION sw_direc;
                read(sensor_fds[0], &sw_direc, sizeof(int) );
                std::cout << "trkDriver:read sensor_fd,sw = {" << sw_num << ", " << 
                                                  sw_direc << "}" << std::endl;
            } else if ( tag == "TRK" ) {
                int nc;
                read(sensor_fds[0], &nc, sizeof(int) );
                char* zn = new char[nc];
                read(sensor_fds[0], zn, nc);
                int v;
                read(sensor_fds[0], &v, sizeof(int) );
                zone_name = zn;
                std::cout << "trkDriver:read sensor_fd, " << zone_name << " = " << v << endl;
            }
            n_event--;
            if ( n_event > 0 ) {
                std::cout << "trkDriver: Read on event pipe, n_event = " << 
                                     n_event << std::endl;
                read(sensor_fds[0], ctag, 4);
            }
        }
//      if ( !done) {
//          yesno = get_yesno( "trkDriver flip(_switches?");
//          if ( yesno == "yes" ) {
//              bool flip_sw = true;
//              while ( flip_sw ) {
//                  SWKey key = get_switch();
//                  if ( key.num != -1) {
//                      switches->set_direction(key);
//                  } else {
//                      flip_sw = false;
//                  }
//              }
//          }
//      }
    }

    delete brk_event;
    delete switches;
    pcp->off();
    delete pcp;
    DemuxAddress* da = DemuxAddress::instance();
    delete da;
    return 0;
}
