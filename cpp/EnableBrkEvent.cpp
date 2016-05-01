
#include "GPIOConfig.h"
#include "EnableBrkEvent.h"
#include "BrkEventSensor.h"

using namespace trk;

trk::EnableBrkEvent::
EnableBrkEvent(int sensor_fd , int& n_event)
{
    GPIOConfig* gpiocfg = GPIOConfig::instance();
    gpio_brk_ = gpiocfg->brk_event_gpio();
    brk_event_sensor_ = new BrkEventSensor( sensor_fd, n_event);
    gpio_brk_->edge_type(RISING);
    gpio_brk_->debounce_time(200);
    gpio_brk_->wait_for_edge(brk_event_sensor_);
    std::cout << "EnableBrkEvent.ctor, Poll started on " << gpio_brk_->number() << std::endl;
}

trk::EnableBrkEvent::
~EnableBrkEvent()
{
    delete gpio_brk_;
}

