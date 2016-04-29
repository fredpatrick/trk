#ifndef TRK_SWITCH_HH
#define TRK_SWITCH_HH

#include "GPIOConfig.h"

namespace trk {

class SwitchSensor;
class DemuxAddress;

class Switch {
    public:
        Switch(int sw_num);
        ~Switch();

        bool            enable_sensors(int sensor_fd);
        bool            set_direction(const SW_DIRECTION& sw_direc);
        void sensor_event(int value, int cout, const SW_DIRECTION& sw_direc);
        SW_DIRECTION    state();
        int             sw_num();
    private:
        GPIOConfig*     gpio_config_;
        int             sw_num_;
        SW_DIRECTION    sw_direc_;
        InputGPIO*      gpio_thru_;
        InputGPIO*      gpio_out_;
        SwitchSensor*   switch_sensor_thru_;
        SwitchSensor*   switch_sensor_out_;
        DemuxAddress*   demux_address_;
};

}

#endif
