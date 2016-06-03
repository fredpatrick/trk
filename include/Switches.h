#ifndef TRK_SWITCHES_HH
#define TRK_SWITCHES_HH

#include "trkutl.h"
#include "Switch.h"

namespace trk {

class Switches {
    public:
        Switches();
        ~Switches();

        bool enable_sensors(int sensor_fd, int& n_event);
        void set_direction(const SWKey& key);
        void manual_set();

        void scan(SW_DIRECTION* sw_state);
        void list_state();
    private:
        Switch*     switch_[6];
};

}

#endif
