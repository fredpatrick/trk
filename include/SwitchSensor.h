#ifndef TRK_SWITCHSENSOR_HH
#define TRK_SWITCHSENSOR_HH

#include "InputSensor.h"
#include "GPIO.h"
#include "GPIOConfig.h"

namespace trk {

class Switch;

class SwitchSensor : public InputSensor
{
    public:
        SwitchSensor(Switch* sw, const SW_DIRECTION& sw_direc, int sensor_fd);
        ~SwitchSensor();

        void event (int ierr, InputGPIO* gpio);
        int     value();
        int     count();
        timeval timeofday();

    private:
        Switch*         switch_;
        SW_DIRECTION    sw_direc_;
        int             sensor_fd_;

        int             value_;
        int             count_;
        timeval         time_of_day_;
};

}

#endif
