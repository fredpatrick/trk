#ifndef TRK_BREAKSENSOR_HH
#define TRK_BREAKSENSOR_HH

#include "InputSensor.h"
#include "GPIO.h"
#include "GPIOConfig.h"

namespace trk {

class JobClock;

class BreakSensor : public InputSensor
{
    public:
        BreakSensor( int sensor_fd, int& n_event);
        ~BreakSensor();

        void event (int ierr, InputGPIO* gpio);
        int     value();
        int     count();
        double  timeofday();

    private:
        int             sensor_fd_;
        int&            n_event_;

        int             value_;
        int             count_;
        double          tm_event_;

        JobClock*       job_clock_;
};

}

#endif
