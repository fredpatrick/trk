

#include "PCPower.h"

using namespace trk;

trk::PCPower* trk::PCPower::instance_ = 0;

trk::PCPower*
trk::PCPower::
instance()
{
    if (!instance_) {
        instance_ = new PCPower();
    }
    return instance_;
}

trk::PCPower::
PCPower()
{
    gpio_pwr_ = new OutputGPIO(60);
}

trk::PCPower::
~PCPower()
{
    instance_ = 0;
    delete gpio_pwr_;
}

int
trk::PCPower::
on()
{
    return gpio_pwr_->value(HIGH);
}

int
trk::PCPower::
off()
{
    return gpio_pwr_->value(LOW);
}

