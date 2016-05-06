#ifndef TRK_ZONE_HH
#define TRK_ZONE_HH


#include <string>
#include "trkutl.h"

namespace trk {

class InputGPIO;
class GPIOConfig;
class TrackSensor;
class DemuxAddress;

class Zone {
    public:
        Zone(const std::string& zone_name);
        ~Zone();

        bool            enable_sensor(int sensor_fd, int& n_event);
        bool            blockit();
        std::string     zone_name();
        bool            state();
    private:
        GPIOConfig*     gpio_config_;
        std::string     zone_name_;
        InputGPIO*      track_gpio_;
        TrackSensor*    track_sensor_;

        int             blk_base_addr_;
        int             blk_index_;
        DemuxAddress*   demux_address_;
};

}

#endif
