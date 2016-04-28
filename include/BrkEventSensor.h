#ifndef TRK_BRKEVENTSENSOR_HH
#define TRK_BRKEVENTSENSOR_HH

#include "InputSensor.h"
#include "GPIO.h"
#include "GPIOConfig.h"

namespace trk {

class JobClock;

class BrkEventSensor : public InputSensor
{
    public:
        BrkEventSensor( int sensor_fd);
        ~BrkEventSensor();

        void event (int ierr, InputGPIO* gpio);
        int     value();
        int     count();
        double  timeofday();

    private:
        int             sensor_fd_;

        int             value_;
        int             count_;
        double          tm_event_;

        JobClock*       job_clock_;
};

}

#endif
