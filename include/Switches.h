#ifndef TRK_SWITCHES_HH
#define TRK_SWITCHES_HH

#include "Switch.h"

namespace trk {

class Switches {
    public:
        Switches();
        ~Switches();

        bool enable_sensors(int sensor_fd);
        void set_direction(int sw_num, const SW_DIRECTION& sw_direc);
        void manual_set();

        void list_state();
    private:
        Switch*     switch_[6];
};

}

#endif
