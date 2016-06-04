
#ifndef TRK_ENABLEBREAKEVENT_HH
#define TRK_ENABLEBREAKEVENT_HH

#include "GPIO.h"

namespace trk {

class BreakSensor;

class EnableBreakEvent {

    public:
        EnableBreakEvent(int sensor_fd , int& n_event);
        ~EnableBreakEvent();

    private:
        InputGPIO*            gpio_brk_;
        BreakSensor*       brk_event_sensor_;
};


}

#endif

