/**
 * Modified by Fred W Patrick: 19 Aug 2015
 *      - Change style
 *      - Create derived classes for input gpios and for output gpios
 *      - Change namespace to trk
  
 * GPIO.cpp  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
 * If you use this code in your work please cite:
 *   Derek Molloy, "Exploring BeagleBone: Tools and Techniques for Building
 *   with Embedded Linux", Wiley, 2014, ISBN:9781118935125.
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European
 * Union Public License (the "EUPL") version 1.1 as published by the
 * European Union. Any use of this Software, other than as authorized
 * under this License is strictly prohibited (to the extent such use
 * is covered by a right of the copyright holder of this Software).
 *
 * This Software is provided under the License on an "AS IS" basis and
 * without warranties of any kind concerning the Software, including
 * without limitation merchantability, fitness for a particular purpose,
 * absence of defects or errors, accuracy, and non-infringement of
 * intellectual property rights other than copyright. This disclaimer
 * of warranty is an essential part of the License and a condition for
 * the grant of any rights to this Software.
 *
 * For more details, see http://www.derekmolloy.ie/
 */

#include "GPIO.h"
#include "InputSensor.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<cstdio>
#include<fcntl.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<pthread.h>
using namespace std;

/************************************************ class GPIO
 *************************************************************/

trk::GPIO::
GPIO(int number)
{
    number_     = number;
    ostringstream s;
    s << "gpio" << number_;
    name_       = string(s.str());
    path_       = GPIO_PATH + name_ + "/";
    export_gpio();
    usleep(250000); // 250ms delay to give Linux time to set up the sysfs structure
}

trk::GPIO::
~GPIO()
{
//  std::cout << "GPIO.dtor" << endl;
    unexport_gpio();
}

int 
trk::GPIO::
export_gpio()
{
   return gpio_write(GPIO_PATH, "export", number_);
}

int
trk::GPIO::
unexport_gpio()
{
   return gpio_write(GPIO_PATH, "unexport", number_);
}

int
trk::GPIO::
value(){
	string input = gpio_read(path_, "value");
	if (input == "0") return 0;
	else              return 1;
}

int 
trk::GPIO::
value(int value)
{
    switch(value){
    case 1: 
        return gpio_write(path_, "value", "1");
    case 0: 
        return gpio_write(path_, "value", "0");
    }
    return -1;
}

int 
trk::GPIO::
gpio_write(string path, string filename, int gpio_parm)
{
   ostringstream s;
   s << gpio_parm;
   return gpio_write(path,filename,s.str());
}

int 
trk::GPIO::
gpio_write(string path, string filename, string gpio_parm)
{
   ofstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   perror("GPIO: write failed to open file ");
  	   return -1;
   }
   fs << gpio_parm;
   fs.close();
   return 0;
}

string 
trk::GPIO::
gpio_read(string path, string filename)
{
   ifstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   perror("GPIO: read failed to open file ");
    }
   string input;
   getline(fs,input);
   fs.close();
   return input;
}


/************************************************ class InputGPIO
 *************************************************************/

trk::InputGPIO::
InputGPIO(int number) : GPIO(number)
{
    gpio_write(path_, "direction", "in");
//  std::cout << "InputGPIO.ctor: gpio direction set " << std::endl;

    debounce_time_ = 0;
    ev_count_ = 0;
    input_sensor_ = NULL;
}

trk::InputGPIO::
~InputGPIO()
{
//  std::cout << "InputGPIO.dtor" << endl;
}

int
trk::InputGPIO::
edge_type(GPIO_EDGE et)
{
    switch(et)
    {
        case NONE: 
            return gpio_write(path_, "edge", "none");
        case RISING: 
            return gpio_write(path_, "edge", "rising");
        case FALLING: 
            return gpio_write(path_, "edge", "falling");
        case BOTH: 
            return gpio_write(path_, "edge", "both");
   }
   return -1;
}

trk::GPIO_EDGE 
trk::InputGPIO::edge_type()
{
    string input = gpio_read(path_, "edge");
    if (input == "rising") 
        return RISING;
    else if (input == "falling") 
        return FALLING;
    else if (input == "both") 
        return BOTH;
    else 
        return NONE;
}

int 
trk::InputGPIO::
wait_for_edge(InputSensor* input_sensor)
{
    thread_running_ = true;
    input_sensor_ = input_sensor;
    if(pthread_create(&thread_, NULL,             // created thread id
                      &threaded_poll,             // routine where thread starts
                      static_cast<void*>(this)))  // argument passed to start routine
    {
    	perror("InputGPIO: Failed to create the poll thread");
    	thread_running_ = false;
    	return -1;
    }
    return 0;
}

void*
trk::InputGPIO::
threaded_poll(void *attr){
    InputGPIO *gpio = static_cast<InputGPIO*>(attr);
    while(gpio->thread_running_){
	gpio->input_sensor_->event(gpio->wait_for_edge(), gpio);
	usleep(gpio->debounce_time_ * 1000);
    }
    return 0;
}

int 
trk::InputGPIO::
wait_for_edge()           //  block waiting for edge - see epoll man page
{
    int     fd;
    if ((fd = open((path_ + "value").c_str(), O_RDONLY | O_NONBLOCK)) == -1) {
       perror("GPIO: Failed to open file");
       return -1;
    }

    int epollfd = epoll_create(1);
    if (epollfd == -1) {
	   perror("GPIO: Failed to create epollfd");
	   return -1;
    }

    struct   epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI; //read operation | edge triggered | urgent data
    ev.data.fd = fd;  // attach the file file descriptor

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
       perror("InputGPIO: Failed to add control interface");
       return -1;
    }

    int count=0;
    while(count<=1)
    {
        int ie = epoll_wait(epollfd, &ev, 1, -1);
	if ( ie == -1)
        {
            perror("GPIO: Poll Wait fail");
            count=5; // terminate loop
	} else {
            count++; // count the triggers up
            ev_count_++;
	}
    }
    close(fd);
    if (count==5) return -1;
    return 0;
}


/************************************************ class OutputGPIO
 *************************************************************/
trk::OutputGPIO::
OutputGPIO(int number) : GPIO(number)
{
    gpio_write(path_, "direction", "out");

    toggle_period_  = 100;
    toggle_number_  = -1; //infinite number
    thread_running_ = false;
}

trk::OutputGPIO::
~OutputGPIO()
{
//  std::cout << "OutputGPIO.dtor" << endl;
}

int 
trk::OutputGPIO::
toggle_output(){
    if ( value() == 1 ) value(0);
    else                value(1);
    return 0;
}

int 
trk::OutputGPIO::
toggle_output(int time)
{ 
    return toggle_output(-1, time); 
}

int 
trk::OutputGPIO::
toggle_output(int number_of_times, int time)
{
    toggle_number_  = number_of_times;
    toggle_period_  = time;
    thread_running_ = true;
    if(pthread_create(&thread_, NULL, 
                      &threaded_toggle, static_cast<void*>(this))){
    	perror("OutputGPIO: Failed to create the toggle thread");
    	thread_running_ = false;
    	return -1;
    }
    return 0;
}

void* 
trk::OutputGPIO::
threaded_toggle(void *attr){
	OutputGPIO *gpio = static_cast<OutputGPIO*>(attr);
	int v = gpio->value(); //find current value
	while(gpio->thread_running_){
		if (v == 0)	gpio->value(1);
		else            gpio->value(0);
		usleep(gpio->toggle_period_ * 500);
		v = 1 - v;
		if(gpio->toggle_number_ > 0 ) gpio->toggle_number_--;
		if(gpio->toggle_number_ == 0) gpio->thread_running_ = false;
	}
	return 0;
}

/*
int GPIO::streamOpen(){
	stream.open((path + "value").c_str());
	return 0;
}
int GPIO::streamWrite(int value){
	stream << value << std::flush;
	return 0;
}
int GPIO::streamClose(){
	stream.close();
	return 0;
}
*/
