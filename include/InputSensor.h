#ifndef TRK_INPUTSENSOR_HH
#define TRK_INPUTSENSOR_HH

#include "GPIO.h"

namespace trk {

class InputSensor
{
    public:
        InputSensor();
        ~InputSensor();

        virtual void operator() (int ierr, InputGPIO* gpio) = 0;
};

}

#endif
