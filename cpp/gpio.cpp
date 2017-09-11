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

#include "gpio.h"
#include "gpio_file_error.h"
#include "inputsensor.h"

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
    gpio_number_     = number;
    gpio_base_path_  = "/sys/class/gpio/"; 
                                                            // write gpio_number to export file
    std::string export_filnam = gpio_base_path_ + "export";
    std::ofstream fs;
    fs.open( export_filnam.c_str() );
    if ( !fs.is_open() ) {
        throw  gpio_file_error("export gpio file open failure", gpio_number_);
    }
    fs << gpio_number_;
    fs.close();
    ::usleep(250000); // 250ms delay to give Linux time to set up the sysfs structure

    std::ostringstream ss;
    ss << "gpio" << gpio_number_;
    gpio_dirnam_ = gpio_base_path_ + ss.str() + "/";
}

trk::GPIO::
~GPIO()
{
//  std::cout << "GPIO.dtor" << endl;
    std::string unexport_filnam = gpio_base_path_ + "unexport";
    std::ofstream fs;
    fs.open( unexport_filnam.c_str() );
    if ( !fs.is_open() ) {
        throw  gpio_file_error("unexport gpio file open failure", gpio_number_);
    }
    fs << gpio_number_;
    fs.close();
}

int
trk::GPIO::
value()
{
    ::lseek (gpio_value_fd_, 0, SEEK_SET);
    char   vc[2];
    int nc = ::read(gpio_value_fd_, vc, 1);
    vc[1] = 0;
    int v = ::atoi(vc);
    return v;
}

/************************************************ class InputGPIO
 *************************************************************/

trk::InputGPIO::
InputGPIO(int number, const std::string& edge_type) : GPIO(number)
{
    std::ofstream fs;
    fs.open((gpio_dirnam_ + "direction").c_str());
    if ( !fs.is_open() ) {
        throw gpio_file_error("open failure for direction file", gpio_number_);
    }
    fs << "in";
    fs.flush();
    fs.close();

    fs.open((gpio_dirnam_ + "edge").c_str());
    if ( !fs.is_open() ) {
        throw gpio_file_error("open failure for edge file", gpio_number_);
    }
    fs << edge_type;
    fs.flush();
    fs.close();

    if ((gpio_value_fd_ = ::open((gpio_dirnam_ + "value").c_str(), 
                                  O_RDONLY | O_NONBLOCK)) == -1) {
        throw gpio_file_error("open failure on value file", gpio_number_);
    }

    debounce_time_ = 0;
    ev_count_ = 0;
    input_sensor_ = NULL;
}

trk::InputGPIO::
~InputGPIO()
{
//  std::cout << "InputGPIO.dtor" << endl;
}
    

/*
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
*/

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
    int epollfd = epoll_create(1);
    if (epollfd == -1) {
	   perror("GPIO: Failed to create epollfd");
	   return -1;
    }

    struct   epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI; //read operation | edge triggered | urgent data
    ev.data.fd = gpio_value_fd_;  // attach the file file descriptor

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, gpio_value_fd_, &ev) == -1) {
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
    if (count==5) return -1;
    return 0;
}


/************************************************ class OutputGPIO
 *************************************************************/
trk::OutputGPIO::
OutputGPIO(int number) : GPIO(number)
{
    std::ofstream fs;
    fs.open((gpio_dirnam_ + "direction").c_str() );
    if ( !fs.is_open() ) {
        throw gpio_file_error("open failure for direction file", gpio_number_);
    }
    fs << "out";
    fs.flush();
    fs.close();

    if ((gpio_value_fd_ = ::open((gpio_dirnam_ + "value").c_str(), O_RDWR)) == -1) {
        throw gpio_file_error("open failure on value file", gpio_number_);
    }

    toggle_period_  = 100;
    toggle_number_  = -1; //infinite number
    thread_running_ = false;
}

trk::OutputGPIO::
~OutputGPIO()
{
//  std::cout << "OutputGPIO.dtor" << endl;
}

void
trk::OutputGPIO::
set_value(int v)
{
    char vc[2];
    vc[1] = 0;
    if ( v == 0 ) vc[0] = '0';
    else if ( v == 1 ) vc[0] = '1';

    ::lseek(gpio_value_fd_, 0, SEEK_SET);
    int nc = ::write(gpio_value_fd_, vc, 1);
}

int 
trk::OutputGPIO::
toggle_output()
{
    if ( value() == 1 ) set_value(0);
    else                set_value(1);
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
		if (v == 0)	gpio->set_value(1);
		else            gpio->set_value(0);
		usleep(gpio->toggle_period_ * 500);
		v = 1 - v;
		if(gpio->toggle_number_ > 0 ) gpio->toggle_number_--;
		if(gpio->toggle_number_ == 0) gpio->thread_running_ = false;
	}
	return 0;
}
