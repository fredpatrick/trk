
#include "GPIOConfig.h"
#include "EnableBrkEvent.h"
#include "BrkEventSensor.h"

using namespace trk;

trk::EnableBrkEvent::
EnableBrkEvent(int sensor_fd )
{
    GPIOConfig* gpiocfg = GPIOConfig::instance();
    gpio_brk_ = gpiocfg->brk_event_gpio();
    brk_event_sensor_ = new BrkEventSensor( sensor_fd);
}

trk::EnableBrkEvent::
~EnableBrkEvent()
{
    delete gpio_brk_;
}

