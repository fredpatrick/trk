#ifndef TRK_SWITCHSENSOR_HH
#define TRK_SWITCHSENSOR_HH

#include "InputSensor.h"
#include "GPIO.h"
#include "trkutl.h"

namespace trk {

class JobClock;

class SwitchSensor : public InputSensor
{
    public:
        SwitchSensor( int sw_num,  const SW_DIRECTION& sw_direc, int sensor_fd, int& n_nevent);
        ~SwitchSensor();

        void event (int ierr, InputGPIO* gpio);
        int     value();
        int     count();
        double  timeofday();
        void    ignore_event(bool s);

    private:
        SW_DIRECTION    sw_direc_;
        int             sw_num_;
        int             sensor_fd_;
        int&            n_event_;
        bool            ignore_event_;

        int             value_;
        int             count_;
        double          tm_event_;
        JobClock*       job_clock_;
};

}

#endif
