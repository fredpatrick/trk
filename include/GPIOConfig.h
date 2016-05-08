
#ifndef TRK_GPIOCONFIG_HH
#define TRK_GPIOCONFIG_HH

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include "trkutl.h"
#include "GPIO.h"

using namespace std;

namespace trk {

struct GPIOData {
    int     gpio_num;
    int     gpio_pin;
    string  direction;
    string  color;
};

class GPIOConfig {
    public: 
        static GPIOConfig* instance();

        ~GPIOConfig();

        int         gpio_num(const string& header_pin);
        GPIO*       pcb_power_gpio();
        GPIO*       demux_address_gpio(const std::string& a);
        InputGPIO*  brk_event_gpio();
        InputGPIO*  switch_gpio(const SWKey& key);
        InputGPIO*  track_gpio(const std::string& zone_name);
        void        clear_gpios();
        std::vector<string> zone_names();
        int         blk_base_addr();
        int         blk_index(const std::string& blk_name);
        std::vector<string> blk_names();

        void    list_header_pins(std::ostream& ostrm);
        void    list_demux_address_pins(std::ostream& ostrm);
        void    list_switch_sensor_pins(std::ostream& ostrm);
        void    list_track_sensor_pins(std::ostream& ostrm);
        void    list_pcb_power_pin(std::ostream& ostrm);
    protected:
        GPIOConfig(const string& cfgfil);
    private:

        map<string, GPIOData>   header_pins_;
        map<string, string>     address_pins_;
        map<SWKey, string>      switch_pins_;
        map<string, string>     track_sensor_pins_;
        map<string, int>        blocker_indicies_;
        string                  pcb_power_pin_;
        string                  brk_event_pin_;

        static GPIOConfig* instance_;
};


}

std::ostream&
operator<<( std::ostream& ostrm, const trk::GPIOData& data);

#endif
