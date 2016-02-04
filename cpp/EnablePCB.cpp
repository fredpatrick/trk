
#include "GPIOConfig.h"
#include "EnablePCB.h"

using namespace trk;

trk::EnablePCB* trk::EnablePCB::instance_ = 0;

trk::EnablePCB*
trk::EnablePCB::
instance()
{
    if (!instance_) {
        instance_ = new EnablePCB();
    }
    return instance_;
}

trk::EnablePCB::
EnablePCB()
{
    GPIOConfig* gpiocfg = GPIOConfig::instance();
    gpio_pwr_ = gpiocfg->pcb_power_gpio();
}

trk::EnablePCB::
~EnablePCB()
{
    instance_ = 0;
    delete gpio_pwr_;
}

int
trk::EnablePCB::
on()
{
    return gpio_pwr_->value(HIGH);
}

int
trk::EnablePCB::
off()
{
    return gpio_pwr_->value(LOW);
}

