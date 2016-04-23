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
        SwitchSensor(Switch* sw, const SW_DIRECTION& sw_direc);
        ~SwitchSensor();

        void operator() (int ierr, InputGPIO* gpio);
        int     value();

    private:
        Switch*         switch_;
        SW_DIRECTION    sw_direc_;
        int             value_;
};

}

#endif
