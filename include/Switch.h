#ifndef TRK_SWITCH_HH
#define TRK_SWITCH_HH

#include "GPIOConfig.h"

namespace trk {

class SwitchSensor;

class Switch {
    public:
        Switch(int sw_num, int sensor_fd);
        ~Switch();

        void sensor_event(int value, int cout, const SW_DIRECTION& sw_direc);
        SW_DIRECTION    state();
        int             sw_num();
    private:
        GPIOConfig*     gpio_config_;
        int             sw_num_;
        InputGPIO*      gpio_thru_;
        InputGPIO*      gpio_out_;
        SwitchSensor*   switch_sensor_thru_;
        SwitchSensor*   switch_sensor_out_;
};

}

#endif
