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

#include "CmdServer.h"
#include "CmdPacket.h"
#include "MsgPacket.h"
#include "BreakEvent.h"
#include "EventDevice.h"
#include "EventBuffer.h"
#include "Drivers.h"
#include "BlockDrivers.h"
#include "BreakDrivers.h"
#include "SwitchDrivers.h"
#include "TrackDrivers.h"
#include "EnablePCB.h"
#include "JobClock.h"
#include "SocketServer.h"
#include "event_device_error.h"
#include "illegal_cmdpacket.h"

trk::CmdServer::
CmdServer(SocketServer* ss)
{
    ss_             = ss;
    drivers_        = Drivers::instance();
    break_drivers_  = drivers_->break_drivers();
    block_drivers_  = drivers_->block_drivers();
    switch_drivers_ = drivers_->switch_drivers();
    track_drivers_  = drivers_->track_drivers();
/*
    MsgPacket mp("Ready to receive messages");
    mp.write(ss_);
    */
}

trk::CmdServer::
~CmdServer()
{
}

int
trk::CmdServer::
enable()
{
    ss_->wait_for_packet(this);
    ss_->wait_for_exit();
    std::cout << "CmdServer::enable, thread ended, deleting drivers" << std::endl;
    delete drivers_;
    EnablePCB::instance()->off();
    exit(0);
}

void
trk::CmdServer::
packet(int ierr)
{
    EventBuffer* cbfr;
    try {
        cbfr = ss_->read();
    } catch ( event_device_error r ) {
        std::cout << "CmdServer.process_commands: " << r.reason() << std::endl;
        double tm_event = JobClock::instance()->job_time();
        BreakEvent event(tm_event);
        event.write_event(ss_);
        std::cout << "CmdServer.process_commands, generating BRK event" << std::endl;
        return;
    }
    CmdPacket* cp;
    try {
        cp = new CmdPacket(cbfr);
    } catch ( illegal_cmdpacket r ) {
        std::cout << "CmdServer.process_commands: " << r.reason() << std::endl;
    }
    std::string command = cp->command();
    std::cout << "CmdServer.process_commands, command = " << command << std::endl;
    if (        command == "break" ) {
        double tm_event = JobClock::instance()->job_time();
        BreakEvent event(tm_event);
        event.write_event(ss_);
    } else if ( command == "scan" ) {
        std::string type = cp->type();
        int         n_item = cp->n_item();
        std::cout << "CmdServer.process_commands, type = " << type << std::endl;
        std::cout << "CmdServer.process_commands, n_item = " << n_item << std::endl;
        for ( int i = 0; i < n_item; i++) {
            if ( type == "track" ) {
                std::pair<int, int> item = cp->item(i);
                item.second = track_drivers_->scan(item.first);
                cp->item(i, item);
            } else if( type == "block" ) {
                std::pair<int, int> item = cp->item(i);
                item.second = block_drivers_->scan(item.first);
                cp->item(i, item);
            } else if ( type == "switch" ) {
                std::pair<int, int> item = cp->item(i);
                item.second = switch_drivers_->scan(item.first);
                cp->item(i, item);
            } else {
                std::cout << "CmdServer.process_commands: unknown type = " << 
                                   type << std::endl;
            }
        }
        cp->write(ss_);
    } else if ( command == "set" ) {
        std::string type = cp->type();
        std::cout << "CmdServer.process_commands, type = " << type << std::endl;
        int         n_item = cp->n_item();
        for ( int i = 0; i < n_item; i++) {
            if ( type == "switch" ) {
                std::pair<int, int> item = cp->item(i);
                switch_drivers_->set(item);
            } else if ( type == "block" ) {
                std::pair<int, int> item = cp->item(i);
                block_drivers_->set(item);
            } else {
                std::cout << "CmdServer.process_commands: unknown type = " << 
                                    type << std::endl;
            }
        }
    }
}