#include "Switch.h"
#include "SwitchSensor.h"
#include "GPIO.h"
#include <iostream>


trk::
Switch::Switch(int sw_num, int sensor_fd)
{
    gpio_config_ = GPIOConfig::instance();
//  std::cout << "Switch.ctor" << std::endl;
    sw_num_ = sw_num;
    SWKey keythru = {sw_num, THRU};
    gpio_thru_  = gpio_config_->switch_gpio(keythru);
    SWKey keyout = {sw_num, OUT};
    gpio_out_   = gpio_config_->switch_gpio(keyout);
//  std:cout << "Switch.ctor: Got the gpios" << endl;

    switch_sensor_thru_ = new SwitchSensor(sw_num, THRU, sensor_fd);
    gpio_thru_->edge_type(BOTH);
    gpio_thru_->debounce_time(200);
    gpio_thru_->wait_for_edge(switch_sensor_thru_);
//  std::cout << "Switch.ctor, Poll started on " << gpio_thru_->number() << 
//                                      " thru position" << endl;

    switch_sensor_out_ = new SwitchSensor(sw_num, OUT, sensor_fd);
    gpio_out_->edge_type(BOTH);
    gpio_out_->debounce_time(200);
    gpio_out_->wait_for_edge(switch_sensor_thru_);
//  std::cout << "Switch.ctor, Poll started on " << gpio_out_->number() << 
//                                      " out position" << endl;
}

trk::Switch::
~Switch()
{
//  std::cout << "Switch.dtor" << std::endl;
    delete gpio_thru_;
    delete gpio_out_;
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

int 
trk::Switch::
sw_num()
{
    return sw_num_;
}
