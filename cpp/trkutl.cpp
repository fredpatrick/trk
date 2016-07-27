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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "trkutl.h"

std::string
trk::get_yesno( const std::string& question ) 
{
    std::string yesno;
    bool got_ans = false;
    while ( !got_ans ) {
        std::cout << "trkDriver: " << question << "(yes/no):" << std::endl;
        std::cin >> yesno;
        if ( yesno == "yes" || yesno == "no" ) {
            got_ans = true;
        } else {
            got_ans = false;
        }
    }
    return yesno;
}

trk::SWKey
trk::get_switch ()
{
    SWKey key;
    bool got_ans = false;
    while ( !got_ans ) {
        std::cout << "get_switch:enter sw_num (-1:5), sw_direc (THRU/OUT): " << std::endl;
        std::cin >> key.num;
        if ( std::cin.fail() ) {
            std::cout << "Bad read on sw_num" << std::endl;
            std::cin.clear();
            std::cin.ignore(100, '\n');
            continue;
        }
        std::cout << key.num << std::endl;
        if ( key.num >= -1 && key.num <= 5 ) {
            if ( key.num == -1 ) {
                got_ans = true;
            } else {
                std::cin >> key.swd;
                if ( key.swd == THRU || key.swd == OUT ) {
                    std::cout << "get_switch, switch key = " << key  << std::endl;
                    got_ans = true;
                } else {
                    got_ans = false;
                }
            }
        }
    }
    return key;
}

int 
trk::make_socket_server(int portno)
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( listen_fd == -1) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in      serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family        = AF_INET;
    serv_addr.sin_addr.s_addr   = htonl(INADDR_ANY);
    serv_addr.sin_port          = htons(portno);
    int ierr = bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr) );
    if ( ierr == -1 ) {
        perror( "bind");
        return -1;
    }
    listen(listen_fd, 10);
    int socket_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);
    if ( socket_fd == -1 ) {
        perror("accept");
        return -1;
    }
    return socket_fd;
}

std::istream& 
operator>>( std::istream& is, trk::SW_DIRECTION& idirec)
{
    std::string tmp;
    if ( is >> tmp ) {
        if ( tmp == "THRU") {
            idirec = trk::THRU;
        } else if ( tmp == "OUT" ) {
            idirec = trk::OUT;
        } else {
            idirec = trk::NOVAL;
        }
    }
    return is;
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::SW_DIRECTION& idirec)
{
    if ( idirec == trk::THRU )       ostrm << "THRU  ";
    else if ( idirec == trk::OUT )   ostrm << "OUT   ";
    else if ( idirec == trk::NOVAL)  ostrm << "NOVAL ";
    return ostrm;
}
std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::TRK_STATE& trkstate)
{
    if      ( trkstate == trk::IDLE) ostrm << " IDLE";
    else if ( trkstate == trk::BUSY) ostrm << " BUSY ";
    return ostrm;
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::BLK_STATE& blkstate)
{
    if      ( blkstate == trk::GO   ) ostrm << "GO   ";
    else if ( blkstate == trk::STOP ) ostrm << "STOP ";
    return ostrm;
}

std::istream& 
operator>>( std::istream& is, trk::TrackState& trkstate)
{
    std::string tmp;
    if ( is >> tmp ) {
        if ( tmp == "LowerLoopCW") {
            trkstate = trk::LowerLoopCW;
        } else if ( tmp == "LowerLoopCCW" ) {
            trkstate = trk::LowerLoopCCW;
        } else if ( tmp == "UpperLoopCW" ) {
            trkstate = trk::UpperLoopCW;
        } else if ( tmp == "UpperLoopCCW" ) {
            trkstate = trk::UpperLoopCCW;
        }
    }
    return is;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::TrackState& trkstate)
{
    if (      trkstate == trk::LowerLoopCW  )   ostrm << "LowerLoopCW";
    else if ( trkstate == trk::LowerLoopCCW )   ostrm << "LowerLoopCCW";
    else if ( trkstate == trk::UpperLoopCW  )   ostrm << "UpperLoopCW";
    else if ( trkstate == trk::UpperLoopCCW )   ostrm << "UpperLoopCCW";
    return ostrm;
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::SWKey& key)
{
    ostrm << "trk::SWKey: " << key.num << '\t' << key.swd;
    return ostrm;
}
