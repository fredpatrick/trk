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

#include "trkutl.h"
#include "socketserver.h"
#include "packetserver.h"
#include "debugcntl.h"
#include "jobclock.h"
#include "packetbuffer.h"
#include "event_device_error.h"
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

trk::SocketServer::
SocketServer(int socket_fd) : socket_fd_(socket_fd)
{
}

trk::SocketServer::
~SocketServer()
{
    std::cout << "socketserver.dtor" << std::endl;
    close ( socket_fd_);
}

int
trk::SocketServer::
write(PacketBuffer* ebfr)
{
    int bfrlen = ebfr->bfrlen();
    char* bfr  = ebfr->bfr();
                                          // This solely for the purpose of recording
    std::string tag = bfr;                //  kind of packet is still being written
    std::cout << "SocketServer.write, tag = " << tag << std::endl;

    int nl = ::write( socket_fd_, &bfrlen, sizeof(int) );
    if ( nl != sizeof(int) ) {
        std::ostringstream ost;
        ost << "SocketServer:write, error writing bfrlen, ns = " << nl;
        throw event_device_error(ost.str() );
    }
    int ns = ::write( socket_fd_, bfr, bfrlen);
    if ( ns != bfrlen) {
        std::ostringstream ost;
        ost << "SocketServer:write, error writing bfr, ns = " << ns;
        throw event_device_error(ost.str() );
    }
    return ns;
}

trk::PacketBuffer* 
trk::SocketServer::
read()
{
    int bfrlen;
    int nl;
    nl = ::read(socket_fd_, &bfrlen, sizeof(int));
    if ( nl == 0 )  {
        std::ostringstream ost;
        ost << "SocketServer:read, socket_fd = " << socket_fd_ << 
                                     ", error reading bfrlen, end of file";
        throw event_device_error(ost.str() );
    }
    char* bfr = new char[bfrlen];
    int ns = ::read( socket_fd_, bfr, bfrlen);
    if ( ns != bfrlen ) {
        std::ostringstream ost;
        ost << "SocketServer:read, error reading bfr, bfrlen = " << bfrlen << ", ns = " << ns;
        throw event_device_error(ost.str() );
    }

    std::string tag = bfr;
    PacketBuffer* ebfr = new PacketBuffer(bfrlen, bfr); 
    delete[] bfr;
    return ebfr;
}

int
trk::SocketServer::
wait_for_packet(PacketServer* packet_server)
{
    std::cout << "SocketServer:wait_for_packed called, socket_fd = " <<
                    packet_server->socket_fd()  << std::endl;
    thread_running_ = true;
    packet_server_     = packet_server;
    if( ::pthread_create(&packet_thread_, NULL,
                         &threaded_poll,
                         static_cast<void*>(this)) ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "SocketServer::wait_for_packet, unable to create pthread, \"" << errmsg << "\"";
        throw event_device_error(ost.str() );
    }
    std::cout << "SocketServer.wait_for_packet returning" << std::endl;
    return 0;
}

void*
trk::SocketServer::
threaded_poll(void* attr)
{
    std::cout << "SocketServer.threaded_poll-begin" << std::endl;
    SocketServer* ss = static_cast<SocketServer*>(attr);
    while ( ss->thread_running_ ) {
        ss->packet_server_->packet(ss->wait_for_packet());
        std::cout << "SocketServer.threaded_poll processed packet" << std::endl;
    }
    return 0;
}

int
trk::SocketServer::
wait_for_packet()
{
    int epollfd = epoll_create(1);
    if ( epollfd == -1 ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "SocketServer::wait_for_packet, unable to create epoll, " <<
                                         "\"" << errmsg << "\"";
        throw event_device_error(ost.str() );
    }
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLPRI;
    ev.data.fd= socket_fd_;

    if ( epoll_ctl( epollfd, EPOLL_CTL_ADD, socket_fd_, &ev) == -1 ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "SocketServer::wait_for_packet, unable to add epoll ctl interface, " <<
                                         "\"" << errmsg << "\"";
        throw event_device_error(ost.str() );
    }

    int ie = epoll_wait(epollfd, &ev, 1, -1);
    if ( ie == -1 ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "SocketServer::wait_for_packet, epoll wait failed, " <<
                                         "\"" << errmsg << "\"";
        throw event_device_error(ost.str() );
    }
    std::cout << "SocketServer::wait_for_packet(), received event" << std::endl;
    return 0;
}

int
trk::SocketServer::
wait_for_exit()
{
    void*   res;
    thread_running_ = false;
    int ie = ::pthread_join(packet_thread_, &res);
    if ( ie == -1 ) {
        char* errmsg = ::strerror(errno);
        std::ostringstream ost;
        ost << "SocketServer::wait_for_exit, pthread join failed, " <<
                                         "\"" << errmsg << "\"";
        throw event_device_error(ost.str() );
    }
    std::cout << "SocketServer.wait_for_exit, after ::pthread_join" << std::endl;
    return 0;
}
