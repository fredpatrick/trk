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

#include "SwitchSensor.h"
#include "SwitchEvent.h"
#include "EventDevice.h"
#include "JobClock.h"


trk::SwitchSensor::
SwitchSensor( int                 sw_num, 
              const SW_DIRECTION& sw_direc, 
              EventDevice*        efd)
{
    job_clock_    = JobClock::instance();
    sw_direc_     = sw_direc;
    sw_num_       = sw_num;
    efd_          = efd;
    ignore_event_ = false;
}

trk::SwitchSensor::
~SwitchSensor()
{
//  std::cout << "SwitchSensor.dtor" << std::endl;
}

void
trk::SwitchSensor::
ignore_event(bool s)
{
    ignore_event_ = s;
}

void
trk::SwitchSensor::
event(int ierr, InputGPIO* gpio)
{
    if ( ignore_event_ ) return;

    int ier = pthread_mutex_lock(&write_event_);
    if ( ier != 0 ) std::cout << "SwitchSensor.event, couldn't lock mutex, ier = " <<
                                 ier << std::endl;
    tm_event_ = job_clock_->job_time();

    value_ = gpio->value();
    count_ = gpio->ev_count();

    std::cout.width(50);
    std::cout << "| ";
    std::cout << "SwitchSensor.event: value = " << value_ << 
            " count = " << count_ << std::endl;

    SwitchEvent* sw_event = new SwitchEvent(tm_event_, 
                                            sw_num_, 
                                            sw_direc_);
    sw_event-> write_event(efd_);
    sw_event->print(50);
    ier = pthread_mutex_unlock(&write_event_);
    if ( ier != 0 ) std::cout << "SwitchSensor.event, couldn't unlock mutex, ier = " <<
                                 ier << std::endl;
}

int
trk::SwitchSensor::
value()
{
    return value_;
}

int
trk::SwitchSensor::
count()
{
    return count_;
}

double
trk::SwitchSensor::
timeofday()
{
    return tm_event_;
}
