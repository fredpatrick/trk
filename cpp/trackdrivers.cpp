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

#include "layoutconfig.h"
#include "trackdrivers.h"
#include "trackdriver.h"
#include "eventdevice.h"

trk::TrackDrivers::
TrackDrivers()
{
    LayoutConfig* layout_config = LayoutConfig::instance();
    sensor_names_ = layout_config->track_sensor_names();
    std::cout << "TrackDrivers.ctor, track_sensors size = " << sensor_names_.size() << std::endl;
    sensor_drivers_.resize( sensor_names_.size() );
    for ( int i = 0; i < sensor_names_.size(); i++) {
        TrackDriver* z  = new TrackDriver(sensor_names_[i]);
        sensor_drivers_[layout_config->track_sensor_index( sensor_names_[i] ) ] = z;
        sensor_indexes_[sensor_names_[i]] = i;
    }
}

trk::TrackDrivers::
~TrackDrivers() {
    for ( int i = 0; i < sensor_drivers_.size(); i++) {
        delete sensor_drivers_[i];
    }
}


bool
trk::TrackDrivers::
enable_sensors(EventDevice* efd )
{
    std::cout << "TrackDrivers.enable_sensors, cout = " << sensor_drivers_.size() << std::endl;
    for ( int i = 0; i < sensor_drivers_.size(); i++) {
        sensor_drivers_[ i ]->enable_sensor(efd);
    }
    return true;
}

int
trk::TrackDrivers::
n_zone() const
{
    return sensor_drivers_.size();
}

trk::TRK_STATE
trk::TrackDrivers::
scan(int i) const
{
    return sensor_drivers_[i]->scan();
}

std::ostream&
trk::operator<<( std::ostream& ostrm, const trk::TrackDrivers& track_drivers)
{
    int n = track_drivers.n_zone();
    for ( int i = 0; i < n; i++) ostrm << track_drivers.scan(i);
    return ostrm;
}
