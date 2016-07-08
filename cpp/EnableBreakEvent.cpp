
#include "GPIOConfig.h"
#include "EnableBreakEvent.h"
#include "BreakSensor.h"

using namespace trk;

trk::EnableBreakEvent::
EnableBreakEvent(int sensor_fd , int& n_event)
{
    GPIOConfig* gpiocfg = GPIOConfig::instance();
    gpio_brk_ = gpiocfg->brk_event_gpio();
    brk_event_sensor_ = new BreakSensor( sensor_fd, n_event);
    gpio_brk_->edge_type(RISING);
    gpio_brk_->debounce_time(200);
    gpio_brk_->wait_for_edge(brk_event_sensor_);
}

trk::EnableBreakEvent::
~EnableBreakEvent()
{
    delete gpio_brk_;
}

