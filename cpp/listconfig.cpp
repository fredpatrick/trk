
#include <iostream>
#include <unistd.h>
#include "gpioconfig.h"

using namespace trk;
using namespace std;

int main()
{
    cout << "listConfig:: Begin" << endl;
    cout.flush();
    GPIOConfig* gpiocfg = trk::GPIOConfig::instance();
    gpiocfg->list_header_pins(cout);
    gpiocfg->list_demux_address_pins(cout);
    gpiocfg->list_switch_sensor_pins(cout);
    gpiocfg->list_track_sensor_pins(cout);
    gpiocfg->list_pcb_power_pin(cout);
    return 0;
}

