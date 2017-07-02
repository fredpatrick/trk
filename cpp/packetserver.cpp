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

#include <pthread.h>
#include "packetserver.h"
#include "cmdpacket.h"
#include "inipacket.h"
#include "msgpacket.h"
#include "debugcntl.h"
#include "filestore.h"
#include "jobclock.h"
#include "demuxaddress.h"
#include "enablepcb.h"
#include "breakevent.h"
#include "eventdevice.h"
#include "packetbuffer.h"
#include "drivers.h"
#include "blockdrivers.h"
#include "breakdrivers.h"
#include "switchdrivers.h"
#include "trackdrivers.h"
#include "enablepcb.h"
#include "socketserver.h"
#include "event_device_error.h"
#include "illegal_cmdpacket.h"

trk::PacketServer::
PacketServer(SocketServer* ss, int debug_level)
{
    ss_             = ss;
    debug_level_ = debug_level;
    ss_->wait_for_packet(this);
    ss_->wait_for_exit();
    std::cout << "PacketServer::enable, thread ended" << std::endl;
    return;
}

trk::PacketServer::
~PacketServer()
{
    std::cout << "#####################################################################";
    std::cout << "PacketServer.dtor, deleting drivers" << std::endl;
    delete block_drivers_;
    delete break_drivers_;
    delete switch_drivers_;
    delete track_drivers_;
    delete DemuxAddress::instance();
    delete EnablePCB::instance();
}

void
trk::PacketServer::
packet(int ierr)
{
    PacketBuffer* pbfr;
    try {
        pbfr = ss_->read();
    } catch ( event_device_error r ) {
        std::cout << "PacketServer.packet: " << r.reason() << std::endl;
        std::cout << "PacketServer.packet, deleting drivers" << std::endl;
        ::pthread_exit(0);
    }

    std::string tag = pbfr->tag();
    if ( tag == "CMD" ) {
        process_cmds(pbfr);
    } else if ( tag == "INI" ) {
        begin_startup(pbfr);
    }
}

void
trk::PacketServer::
process_cmds(PacketBuffer* pbfr)
{

    CmdPacket* cp;
    try {
        cp = new CmdPacket(pbfr);
    } catch ( illegal_cmdpacket r ) {
        std::cout << "PacketServer.packet: " << r.reason() << std::endl;
    } 
    std::string command = cp->command();
    if (        command == "break" ) {
        std::cout << "#####################################################################";
        std::cout << "PacketServer.process_cmds, break" << std::endl;
        ::pthread_exit(0);
    } else if ( command == "finish_startup" ) {
        finish_startup(pbfr);
    } else if ( command == "scan" ) {
        std::string type = cp->type();
        int         n_item = cp->n_item();
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
                std::cout << "PacketServer.packet: unknown type = " << 
                                   type << std::endl;
            }
        }
        cp->write(ss_);
    } else if ( command == "set" ) {
        std::string type = cp->type();
        int         n_item = cp->n_item();
        for ( int i = 0; i < n_item; i++) {
            if ( type == "switch" ) {
                std::pair<int, int> item = cp->item(i);
                switch_drivers_->set(item);

            } else if ( type == "block" ) {
                std::pair<int, int> item = cp->item(i);
                block_drivers_->set(item);
            } else {
                std::cout << "PacketServer.packet: unknown type = " << 
                                    type << std::endl;
            }
        }
    }
}

void
trk::PacketServer::
begin_startup(PacketBuffer* pbfr)
{
    IniPacket* ip = new IniPacket(pbfr);
    std::string type = ip->type();

    std::cout << "PacketServer.begin_startup, ################################# " 
                    << " type = " << type << std::endl;
    if ( type != "tod" ) {
        std::cout << "PacketServer.begin_startup, Error!! Do not know type = " 
                         << type << std::endl;
        return;
    }

    std::string todts = ip->tod_timestamp();
    std::cout << "PacketServer.startup_process, ################################# "
                        << " tod = " << todts << std::endl;
    delete ip;
    JobClock* job_clock = JobClock::instance();
    job_clock->tod_timestamp(todts);

    ip = new IniPacket("btm");
    ip->t0( job_clock->base_time() );
    ip->t1( job_clock->job_time() );
    ip->write(ss_);
    delete ip;
    return;
}

void
trk::PacketServer::
finish_startup(PacketBuffer* pbfr)
{
    std::string homedir = "/home/fredpatrick/";
    std::string cfg_filnam = homedir + "wrk/cfg/layout_config.txt";
    std::string todts = JobClock::instance()->tod_timestamp();
    std::string dbg_filnam = homedir + "wrk/log/" + todts + "_trkDriver.txt";
    FileStore* fs = FileStore::instance();
    fs->cfgfil(cfg_filnam);
    fs->dbgfil(dbg_filnam);
    DebugCntl* dbg = DebugCntl::instance();
    dbg->level(debug_level_);

    EnablePCB* pcb = EnablePCB::instance();
    pcb->on();

    block_drivers_ = new BlockDrivers();
    block_drivers_->enable_sensors(ss_);
    break_drivers_  = new BreakDrivers();
    break_drivers_->enable_sensors(ss_);
    switch_drivers_ = new SwitchDrivers();
    switch_drivers_->enable_sensors(ss_);
    track_drivers_  = new TrackDrivers();
    track_drivers_->enable_sensors(ss_);
}
