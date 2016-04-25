
#include <iostream>

#include "DemuxAddress.h"

trk::DemuxAddress* trk::DemuxAddress::instance_ = 0;

trk::DemuxAddress*
trk::
DemuxAddress::instance()
{
    if( !instance_ ) {
        instance_ = new DemuxAddress();
    }
    return instance_;
}

trk::
DemuxAddress::DemuxAddress()
{
    GPIOConfig* gpio_config = GPIOConfig::instance();
    gpios_[0] = gpio_config->demux_address_gpio("a0");
    gpios_[1] = gpio_config->demux_address_gpio("a1");
    gpios_[2] = gpio_config->demux_address_gpio("a2");
    gpios_[3] = gpio_config->demux_address_gpio("a3");
    gpios_[4] = gpio_config->demux_address_gpio("a4");

    for ( int i = 0; i < 6; i++ ) {
        std::cout << "DemuxADDRESS:ctor: n = " << gpios_[i]->number() << std::endl;
    }
}

trk::DemuxAddress::
~DemuxAddress()
{
    delete gpios_;
    instance_ = 0;
}

bool
trk::
DemuxAddress::set(int sw_num, SW_DIRECTION& sw_direc)
{
    std::cout << "DemuxAddress.set: sw_num = " << sw_num << 
                                " sw_direc = " << sw_direc << std::endl;
    if ( sw_num < 0 || sw_num > 5|| sw_direc == NOVAL) return false;

    int line_index = sw_num * 2 + sw_direc;
    std::bitset<8 * sizeof(int)> b = line_index;
    gpios_[4]->value(HIGH);
    gpios_[0]->value( GPIO_VALUE( (int)b[0] ));
    gpios_[1]->value( GPIO_VALUE( (int)b[1] ));
    gpios_[2]->value( GPIO_VALUE( (int)b[2] ));
    gpios_[3]->value( GPIO_VALUE( (int)b[3] ));
    gpios_[4]->value(LOW);

    usleep(50000);
    gpios_[4]->value(HIGH);
    return true;
}
