#ifndef TRK_ZONES_HH
#define TRK_ZONES_HH

#include <map>
#include <vector>
#include <string>

namespace trk {

class GPIOConfig;
class Zone;

class Zones {
    public:
        Zones();
        ~Zones();

        bool      enable_sensors(int sensor_fd, int& n_event);
        int       n_zone() const;
        TRK_STATE get_state(int i ) const;

    private:
        std::map<std::string, int>    zone_indexes_;;
        std::vector<std::string>      zone_names_;
        std::vector<Zone*>              zones_;

        GPIOConfig* gpio_config_;
}; 

std::ostream&
operator<<( std::ostream& ostrm, const trk::Zones& zones);


}

#endif
