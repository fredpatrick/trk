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
#include "switchdrivers.h"
#include "eventdevice.h"
#include "trkutl.h"

trk::SwitchDrivers::
SwitchDrivers()
{
    std::cout << "SwitchDrivers:ctor" << std::endl;
    for ( int i = 0; i < 6; i++) {
        switch_[i] = new SwitchDriver(i);
        //switch_[i]->set(THRU);
    }
}

trk::SwitchDrivers::
~SwitchDrivers()
{
//  std::cout << "SwitchDrivers:dtor" << std::endl;
    for ( int i = 0; i < 6; i++) {
        delete switch_[i];
    }
}

bool
trk::SwitchDrivers::
enable_sensors(EventDevice* efd)
{
    for ( int i = 0; i < 6; i++) {
        switch_[i]->enable_sensors(efd);
    }
    return true;
}

void
trk::SwitchDrivers::
set( const std::pair<int, int>& item)
{
    switch_[item.first]->set(item.second);
}

int
trk::SwitchDrivers::
scan(int i)
{
    switch_[i]->scan();
}

/*
bool
trk::SwitchDrivers::
manual_set()
{
    SW_DIRECTION sw_state[6];
    bool done = false;
    SWKey key = get_switch_key();
    if ( key.num == -1 ) {
        return true;
    } else {
        set_direction(key);
        return false;
    }
}

trk::SWKey
trk::SwitchDrivers::
get_switch_key()
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
*/

void
trk::SwitchDrivers::
list_state()
{
    for ( int i = 0; i < 6; i++) {
        std::cout << switch_[i]->scan();
    }
    std::cout << std::endl;
}

trk::SwitchDriver*
trk::SwitchDrivers::
swtch(int i) const
{
    return switch_[i];
}

std::ostream& 
trk::operator<<( std::ostream& ostrm, const trk::SwitchDrivers& switches)
{
    for ( int i = 0; i < 6; i++) {
        ostrm << switches.swtch(i)->scan();
    }
    return  ostrm;
}

