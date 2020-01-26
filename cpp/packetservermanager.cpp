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

#include "packetservermanager.h"
#include "packetbuffer.h"
#include "esppacketserver.h"
#include "cntlpacketserver.h"
#include "event_device_error.h"
#include "filestore.h"
#include "jobclock.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <sstream>

trk::PacketServerManager* trk::PacketServerManager::instance_ = 0;

trk::PacketServerManager::
PacketServerManager()
{
    listen_fd_           = -1;
    shutdown_            = false;
    cntl_packet_server_  = 0;
}

trk::PacketServerManager*
trk::PacketServerManager::
instance()
{
    if ( instance_ == 0 ) {
        instance_ = new PacketServerManager();
    }
    return instance_;
}

void
trk::PacketServerManager::
listen_for_connection(int portno)
{
    if ( listen_fd_ != -1 ) return;

    portno_ = portno;
    std::cout << "PacketServerManager.listen_for_connection, portno = " << portno_ << std::endl;
    listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if ( listen_fd_ == -1) {
        ::perror("socket");
        // need to throw exception here
    }
    std::cout << "PacketServerManager.listen_for_connection, listen_fd = " << 
                                                           listen_fd_ << std::endl;
    struct sockaddr_in      serv_addr;
    ::memset(&serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family        = AF_INET;
    serv_addr.sin_addr.s_addr   = htonl(INADDR_ANY);
    serv_addr.sin_port          = htons(portno_);
    int ierr = ::bind(listen_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr) );
    if ( ierr == -1 ) {
        ::perror( "bind");
        throw event_device_error("PacketServerManagerlisten socket");
    }

    std::cout << "PacketServerManager.listen_for_connection, listening on portno = " << 
                                            portno_ << std::endl;
    ::listen(listen_fd_, 10);
    std::cout << "PacketServerManager.listen_for_connection, listen_fd = " << 
                                                           listen_fd_ << std::endl;

    while (!shutdown_) {
        std::cout << "PacketServerManager.listen_for_connection, listen_fd = " << 
                                                               listen_fd_ << std::endl;
        int socket_fd = accept(listen_fd_, (struct sockaddr*)NULL, NULL);
        if ( socket_fd == -1 ) {
            std::cout << "PacketServerManager.listen_for_connection, accept error: " << 
                                                  ::strerror(errno) << std::endl;
            throw event_device_error("PacketServerManager.listen_for_connection accept error");
        }
        std::cout << "PacketServerManager.listen_for_connection, connected on portno = " << 
                                   portno_ << ", socket_fd = " << socket_fd << std::endl;
        int bfrlen;
        int nl;
        nl = ::read(socket_fd, &bfrlen, sizeof(int));
        if ( nl == 0 )  {
            std::ostringstream ost;
            ost << "PacketServerManager:read, socket_fd = " << socket_fd << 
                                         ", error reading bfrlen, end of file";
            throw event_device_error(ost.str() );
        }
        char* bfr = new char[bfrlen];
        int ns = ::read( socket_fd, bfr, bfrlen);
        if ( ns != bfrlen ) {
            std::ostringstream ost;
            ost << "PacketServerManager:read, error reading bfr, bfrlen = " << 
                                         bfrlen << ", ns = " << ns;
            throw event_device_error(ost.str() );
        }

        PacketBuffer* ebfr = new PacketBuffer(bfrlen, bfr); 
        std::string ss_type = ebfr->tag();
        std::cout << "PacketServerManager: Received tag, ss_type = " << ss_type << std::endl;
        std::cout << "PacketServerManager:               bfrlen  = " << 
                                     ebfr->bfrlen() << std::endl;
        delete[] bfr;

        PacketServer* ps;
        if        ( ss_type == "CNTL" ) {
            if ( !cntl_packet_server_ ) {
                ps = new CNTLPacketServer(socket_fd, shutdown_);
                cntl_packet_server_ = dynamic_cast<CNTLPacketServer*>(ps);
            }
        } else if ( ss_type == "ESP" ) {
            int srvnum = ebfr->intdat();
            std::cout << "PacketServerManager:               srvnum = " << srvnum << std::endl;
            ps = new ESPPacketServer(socket_fd, shutdown_);
                packet_servers_.push_back(ps);
        } else {
        }
    }
    std::cout << "PacketServerManager::listening, loop exitted" << std::endl;
}

trk::PacketServerManager::
~PacketServerManager()
{
    
}

void 
trk::PacketServerManager::
stop_listening()
{
    std::cout << "PacketServerManager::stop_listening, listen_fd = " << listen_fd_ << std::endl;
    ::shutdown(listen_fd_, SHUT_RDWR);
}
