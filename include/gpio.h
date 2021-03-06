/*
 * Modified by Fred  W Patrick: 19 Aug 2015

 * GPIO.h  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
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


#ifndef TRK_GPIO_HH_
#define TRK_GPIO_HH_
#include<string>
#include<fstream>

namespace trk {

class InputSensor;

enum    GPIO_EDGE{ NONE, RISING, FALLING, BOTH };

class GPIO {

    public:
	GPIO(int number);               //constructor will export the pin
	~GPIO();                        //destructor will unexport the pin

	int number() { return gpio_number_; }

	int         value();

    protected:
	int         gpio_number_;
        int         gpio_value_fd_;
        std::string gpio_base_path_;
        std::string gpio_dirnam_;
};

class InputGPIO : public GPIO
{
    public:
        InputGPIO(int number,const std::string& edge_type);
        ~InputGPIO();

        void        debounce_time(int time){ debounce_time_ = time;}
        int         ev_count() { return ev_count_;}
        int         wait_for_edge(InputSensor* );
        int         wait_for_edge();
    private:
        static void* threaded_poll(void* attr);
        int          debounce_time_;
        int          ev_count_; 
        pthread_t    thread_;
        bool         thread_running_;
        InputSensor* input_sensor_;
};

class OutputGPIO : public GPIO
{
    public:
        OutputGPIO(int number);
        ~OutputGPIO();

	void        set_value(int v);

        int toggle_output();
        int toggle_output(int time);
        int toggle_output(int number_of_times, int time);

    private:
        static void* threaded_toggle(void* attr);

        int          toggle_number_;
        int          toggle_period_;
        bool         thread_running_;
        pthread_t    thread_;

};


} /* namespace trk */

#endif /* TRK_GPIO_H_ */
