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

#include "BlockSensor.h"
#include "BlockEvent.h"
#include "EventDevice.h"
#include "trkutl.h"
#include "JobClock.h"

trk::BlockSensor::
BlockSensor( EventDevice*       efd,
             int&               n_event, 
             const std::string& blk_name) : n_event_(n_event)
{
    job_clock_    = JobClock::instance();
    efd_          = efd;
    ignore_event_ = false;
    blk_name_     = blk_name;
//  std::cout << "BlockSensor.ctor:" ;

}

trk::BlockSensor::
~BlockSensor()
{
//  std::cout << "BlockSensor.dtor" << std::endl;
}

void
trk::BlockSensor::
ignore_event()
{
    ignore_event_ = true;
}

void
trk::BlockSensor::
event(int ierr, InputGPIO* gpio)
{
    if ( ignore_event_ ) return;

    int ier = pthread_mutex_lock(&write_event_);
    if ( ier != 0 ) std::cout << "BlockSensor.event, couldn't lock mutex, ier = " <<
                                ier << std::endl;
    tm_event_ = job_clock_->job_time();
    n_event_++;
    value_ = gpio->value();
    BLK_STATE blk_status;
    if      ( value_ == 0 ) blk_status = GO;
    else if ( value_ == 1 ) blk_status = STOP;
    count_ = gpio->ev_count();
    std::cout.width(50);
    std::cout << "| ";
    std::cout << "BlockSensor.event. n_event = " << n_event_ << " - " << tm_event_ << std::endl;

    BlockEvent* blk_event = new BlockEvent(tm_event_, 
                                           blk_name_, 
                                           blk_status);
    blk_event->write_event(efd_);
    blk_event->print(50);
    ier = pthread_mutex_unlock(&write_event_);
    if ( ier != 0 ) std::cout << "BlockSensor.event, couldn't unlock mutex, ier = " <<
                                ier << std::endl;
}

int
trk::BlockSensor::
value()
{
    return value_;
}

int
trk::BlockSensor::
count()
{
    return count_;
}

double
trk::BlockSensor::
timeofday()
{
    return tm_event_;
}
