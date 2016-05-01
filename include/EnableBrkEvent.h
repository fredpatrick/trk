
#ifndef TRK_ENABLEBRKEVENT_HH
#define TRK_ENABLEBRKEVENT_HH

#include "GPIO.h"

namespace trk {

class BrkEventSensor;

class EnableBrkEvent {

    public:
        EnableBrkEvent(int sensor_fd , int& n_event);
        ~EnableBrkEvent();

    private:
        InputGPIO*            gpio_brk_;
        BrkEventSensor*       brk_event_sensor_;
};


}

#endif

