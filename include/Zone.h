#ifndef TRK_ZONE_HH
#define TRK_ZONE_HH


#include <string>
#include "trkutl.h"

namespace trk {

class InputGPIO;
class TrackSensor;

class Zone {
    public:
        Zone(const std::string& zone_name);
        ~Zone();

        bool            enable_sensor(int sensor_fd, int& n_event);
        std::string     zone_name();
        TRK_STATE       state();
    private:
        std::string     zone_name_;
        InputGPIO*      track_gpio_;
        TrackSensor*    track_sensor_;
};

}

#endif
