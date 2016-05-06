#ifndef TRK_ZONES_HH
#define TRK_ZONES_HH

#include <map>
#include <string>

namespace trk {

class GPIOConfig;
class Zone;

class Zones {
    public:
        Zones();
        ~Zones();

        bool enable_sensors(int sensor_fd, int& n_event);
        bool blockit( const std::string& zone_name);

    private:
        std::map<std::string,Zone*> zones_;
        std::vector<std::string>      zone_names_;

        GPIOConfig* gpio_config_;
}; 

}

#endif
