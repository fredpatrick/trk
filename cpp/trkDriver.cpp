/*
 * ============================================================================
 *                   The XyloComp Software License, Version 1.1
 * ============================================================================
 * 
 *    Copyright (C) 2016 XyloComp Inc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modifica-
 * tion, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of  source code must  retain the above copyright  notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The end-user documentation included with the redistribution, if any, must
 *    include  the following  acknowledgment:  "This product includes  software
 *    developed  by  XyloComp Inc.  (http://www.xylocomp.com/)." Alternately, 
 *    this  acknowledgment may  appear in the software itself,  if
 *    and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The name "XyloComp" must not be used to endorse  or promote  products 
 *    derived  from this  software without  prior written permission. 
 *    For written permission, please contact fred.patrick@xylocomp.com.
 * 
 * 5. Products  derived from this software may not  be called "XyloComp", 
 *    nor may "XyloComp" appear  in their name,  without prior written 
 *    permission  of Fred Patrick
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS  FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED.  IN NO  EVENT SHALL
 * XYLOCOMP INC. OR ITS CONTRIBUTORS  BE LIABLE FOR  ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLU-
 * DING, BUT NOT LIMITED TO, PROCUREMENT  OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR  PROFITS; OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON
 * ANY  THEORY OF LIABILITY,  WHETHER  IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING  NEGLIGENCE OR  OTHERWISE) ARISING IN  ANY WAY OUT OF THE  USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

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
#include "trkutl.h"

using namespace trk;

int main() {

    std::cout << "BeagleBoneBlack driver for exercising trk cicuits" <<std::endl;

    JobClock* job_clock = trk::JobClock::instance();
    std::cout << *job_clock << std::endl;

    int socket_fd = 0;
    std::string yesno = "";
    yesno = get_yesno( "Create socket for TCP connection?" );
    if ( yesno == "yes" ) socket_fd = make_socket_server(17303);
    if (  socket_fd ) {
        double t0 = job_clock->base_time();
        double t1 = job_clock->job_time();
        write(socket_fd, &t0, sizeof(double) );
        write(socket_fd, &t1, sizeof(double) );
    }
    
    EnablePCB* pcp = EnablePCB::instance();
    std::cout << "Power controler created " << std::endl;

    yesno = get_yesno ( "trkDriver:Turn power on?" );
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
