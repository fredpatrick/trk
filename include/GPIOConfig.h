#ifndef TRK_GPIOCONFIG_HH
#define TRK_GPIOCONFIG_HH

#include <fstream>
#include <map>
#include <string>
#include "GPIO.h"

using namespace std;

namespace trk {

enum SW_DIRECTION {THRU, OUT, NOVAL };

struct GPIOData {
    int     gpio_num;
    int     gpio_pin;
    string  direction;
    string  color;
};

struct SWKey {
    int             num; 
    SW_DIRECTION    swd;

    bool
    operator< (const SWKey& k2) const
    {
        int a = num * 2 + static_cast<int>(swd);;
        int b = k2.num * 2 + static_cast<int>(k2.swd);;
        if ( a < b) return true;
        else        return false;
    }
};

class GPIOConfig {
    public: 
        static GPIOConfig* instance();

        ~GPIOConfig();

        int gpio_num(const string& header_pin);;
        GPIO*  pcb_power_gpio();

        void    list_header_pins(std::ostream& ostrm);
        void    list_demux_address_pins(std::ostream& ostrm);
        void    list_switch_sensor_pins(std::ostream& ostrm);
        void    list_track_sensor_pins(std::ostream& ostrm);
        void    list_pcb_power_pin(std::ostream& ostrm);
    protected:
        GPIOConfig(const string& cfgfil);
    private:

        map<string, GPIOData>    header_pins_;
        map<string, string>      address_pins_;
        map<SWKey, string>      switch_pins_;
        map<string, string>      track_sensor_pins_;
        string                   pcb_power_pin_;

        static GPIOConfig* instance_;
};


}

std::istream& 
operator>>( std::istream& is, trk::SW_DIRECTION& idirec);

std::ostream&
operator<<( std::ostream& ostrm, const trk::SW_DIRECTION& idirec);

std::ostream&
operator<<( std::ostream& ostrm, const trk::GPIOData& data);

std::ostream&
operator<<( std::ostream& ostrm, const trk::SWKey& kwy);

#endif
