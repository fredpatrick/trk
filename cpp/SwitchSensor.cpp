

#include "SwitchSensor.h"
#include "Switch.h"


trk::
SwitchSensor::SwitchSensor(Switch* sw, const SW_DIRECTION& sw_direc)
{
    switch_     = sw;
    sw_direc_   = sw_direc;
}

trk::
SwitchSensor::~SwitchSensor()
{}

void
trk::
SwitchSensor::operator() (int ierr, InputGPIO* gpio)
{
    value_ = (int)gpio->value();
    int count = gpio->ev_count();
}

int
trk::SwitchSensor::
value()
{
    return value_;
}
