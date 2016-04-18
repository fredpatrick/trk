
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
    cout << "GPIOConfig::GPIOConfig " << cfgfil << " opened" << endl;
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

trk::GPIO*
trk::
GPIOConfig::demux_address_gpio(const std::string& a)
{
    std::string pin_name = address_pins_[a];
    GPIOData data = header_pins_[pin_name];
    return new OutputGPIO(data.gpio_num);
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

std::istream& 
operator>>( std::istream& is, trk::SW_DIRECTION& idirec)
{
    std::string tmp;
    if ( is >> tmp ) {
        if ( tmp == "THRU") {
            idirec = trk::THRU;
        } else if ( tmp == "OUT" ) {
            idirec = trk::OUT;
        } else {
            idirec = trk::NOVAL;
        }
    }
    return is;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::SW_DIRECTION& idirec)
{
    if ( idirec == trk::THRU )      ostrm << "THRU";
    else if ( idirec == trk::OUT )   ostrm << "OUT";
    else if ( idirec == trk::NOVAL)  ostrm << "NOVAL";
    return ostrm;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::GPIOData& data)
{
    ostrm << data.gpio_num << '\t' << data.gpio_pin << '\t' << 
                                     data.direction << '\t' << data.color;
    return ostrm;
}

std::ostream&
operator<<( std::ostream& ostrm, const trk::SWKey& key)
{
    ostrm << "trk::SWKey: " << key.num << '\t' << key.swd;
    return ostrm;
}
