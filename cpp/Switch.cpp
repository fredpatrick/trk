#include "Switch.h"
#include "SwitchSensor.h"
#include "GPIO.h"
#include <iostream>


trk::
Switch::Switch(int sw_num)
{
    sw_num_ = sw_num;
    SWKey keythru = {sw_num, THRU};
    gpio_thru_  = gpio_config_->switch_gpio(keythru);
    SWKey keyout = {sw_num, OUT};
    gpio_out_   = gpio_config_->switch_gpio(keyout);

    switch_sensor_thru_ = new SwitchSensor(this, THRU);
    gpio_thru_->edge_type(BOTH);
    gpio_thru_->debounce_time(200);
    gpio_thru_->wait_for_edge(switch_sensor_thru_);
    std::cout << "trkDriver, Poll started on 79, thru position" << endl;

    switch_sensor_out_ = new SwitchSensor(this, OUT);
    gpio_out_->edge_type(BOTH);
    gpio_out_->debounce_time(200);
    gpio_out_->wait_for_edge(switch_sensor_thru_);
}

void
trk::
Switch::sensor_event(int value, int count,const SW_DIRECTION& sw_direc)
{
    cout << "Switch, value = " << value << endl;
    cout << "Switch, count = " << count << endl;
}
    
trk::SW_DIRECTION
trk::Switch::
state()
{
    int sw_thru = switch_sensor_thru_->value();
    int sw_out  = switch_sensor_out_->value();

    if     ( sw_thru == 1 && sw_out == 0 ) return THRU;
    else if ( sw_thru == 0 && sw_out == 1 ) return OUT;
    else return NOVAL;
}
