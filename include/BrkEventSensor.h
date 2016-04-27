#ifndef TRK_BRKEVENTSENSOR_HH
#define TRK_BRKEVENTSENSOR_HH

#include "InputSensor.h"
#include "GPIO.h"
#include "GPIOConfig.h"

namespace trk {

class BrkEventSensor : public InputSensor
{
    public:
        BrkEventSensor( int sensor_fd);
        ~BrkEventSensor();

        void event (int ierr, InputGPIO* gpio);
        int     value();
        int     count();
        timeval timeofday();

    private:
        int             sensor_fd_;

        int             value_;
        int             count_;
        timeval         time_of_day_;
};

}

#endif
