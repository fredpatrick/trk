
#include <iostream>
#include "GPIOConfig.h"

using namespace std;
using namespace trk;

trk::GPIOConfig* trk::GPIOConfig::instance_ = 0;

trk::GPIOConfig*
trk::
GPIOConfig::instance()
{
    string cfgfil = "./gpio_config";
    if ( !instance_ ) {
        instance_ = new GPIOConfig(cfgfil);
    }
    return instance_;
}

trk::
GPIOConfig::GPIOConfig(const string& cfgfil)
{
    ifstream from(cfgfil.c_str() );
//  cout << "GPIOConfig::GPIOConfig " << cfgfil << " opened" << endl;
    string      pin_name;
    GPIOData    data;
    char        cline[120];

    while (!from.eof() ) {
        string tag = "";
        from >> tag;
        if  (tag[0] == '#' ) {
            from.getline(cline, 120);
        } else if ( tag == "HDR" ) {
            from >> pin_name >> data.gpio_num >> data.gpio_pin >> data.direction >> data.color;
            header_pins_[pin_name] = data;
        } else if ( tag == "ADDR") {
            string dmx_a;
            from >> dmx_a >> pin_name;
            address_pins_[dmx_a] = pin_name;
        } else if ( tag == "SW")   {
            SWKey key;
            from >> key.num >> key.swd;
            from >> pin_name;
            switch_pins_[key] = pin_name;
        } else if ( tag == "TRK")  {
            string zone_name;
            from >> zone_name >> pin_name;
            track_sensor_pins_[zone_name] = pin_name;
        } else if ( tag == "PCB")  {
            from >> pcb_power_pin_;
        } else if ( tag == "BRKSIG") {
            from >> brk_event_pin_;
        }
    }
}

trk::GPIO* 
trk::
GPIOConfig::pcb_power_gpio()
{
    GPIOData data;
    data = header_pins_[pcb_power_pin_];
    return new OutputGPIO(data.gpio_num);
}

trk::InputGPIO*
trk::GPIOConfig::
brk_event_gpio()
{
    GPIOData data = header_pins_[brk_event_pin_];
    return new InputGPIO(data.gpio_num);
}

trk::GPIO*
trk::GPIOConfig::
demux_address_gpio(const std::string& a)
{
    std::string pin_name = address_pins_[a];
    GPIOData data = header_pins_[pin_name];
    return new OutputGPIO(data.gpio_num);
}

trk::InputGPIO*
trk::GPIOConfig::
switch_gpio(const SWKey& key)
{
    std::string pin_name = switch_pins_[key];
    GPIOData    d = header_pins_[pin_name];
    return new InputGPIO( d.gpio_num);
}

trk::InputGPIO*
trk::GPIOConfig::
track_gpio(const std::string& zone_name)
{
    std::string pin_name = track_sensor_pins_[zone_name];
    GPIOData    d = header_pins_[pin_name];
    return new InputGPIO( d.gpio_num);
}

std::vector<string>
trk::GPIOConfig::
zone_names()
{
    std::vector<string> zns;
    typedef std::map<std::string, std::string>::const_iterator CI;
    for ( CI p = track_sensor_pins_.begin(); p != track_sensor_pins_.end(); p++) {
        zns.push_back( p->first);
    }
    return zns;
}

void
trk::GPIOConfig::
clear_gpios()
{
    typedef map<string, GPIOData>::const_iterator CI;
    for (CI p = header_pins_.begin(); p!= header_pins_.end(); ++p) {
        GPIOData data = p->second;
        GPIO* gpio = new GPIO( data.gpio_num);
        delete gpio;
    }
}

void
trk::
GPIOConfig::list_header_pins(std::ostream& ostrm)
{
    ostrm << "GPIOConfig::list_header_pins" << endl;
    typedef map<string, GPIOData>::const_iterator CI;
    for (CI p = header_pins_.begin(); p!= header_pins_.end(); ++p) {
        ostrm << p->first << '\t' << p->second << '\n';
    }
}

void
trk::
GPIOConfig::list_demux_address_pins(std::ostream& ostrm)
{
    ostrm << "GPIOConfig::list_demux_address_pins" << endl;
    typedef map<string, string>::const_iterator CI;
    for (CI p = address_pins_.begin(); p!= address_pins_.end(); ++p) {
        ostrm << p->first << '\t' << p->second << '\n';
    }
}

void
trk::
GPIOConfig::list_switch_sensor_pins(std::ostream& ostrm)
{
    ostrm << "GPIOConfig::list_switch_sensor_pins" << endl;
    typedef map<SWKey, string>::const_iterator CI;
    for (CI p = switch_pins_.begin(); p!= switch_pins_.end(); ++p) {
        ostrm << p->first << '\t' << p->second << '\n';
    }
}

void
trk::
GPIOConfig::list_track_sensor_pins(std::ostream& ostrm)
{
    ostrm << "GPIOConfig::list_track_sensor_pins" << endl;
    typedef map<string, string>::const_iterator CI;
    for (CI p = track_sensor_pins_.begin(); p!= track_sensor_pins_.end(); ++p) {
        ostrm << p->first << '\t' << p->second << '\n';
    }
}

void
trk::
GPIOConfig::list_pcb_power_pin(std::ostream& ostrm)
{
    ostrm << "GPIOConfig::pcb_power_pin: ";
    ostrm << pcb_power_pin_ << endl;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::GPIOData& data)
{
    ostrm << data.gpio_num << '\t' << data.gpio_pin << '\t' << 
                                     data.direction << '\t' << data.color;
    return ostrm;
}
