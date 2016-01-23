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
using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"

namespace trk {

typedef int (*CallbackType)(int, void*);

enum    GPIO_DIRECTION{ INPUT, OUTPUT };
enum    GPIO_VALUE{ LOW=0, HIGH=1 };
enum    GPIO_EDGE{ NONE, RISING, FALLING, BOTH };

class GPIO {

    public:
	GPIO(int number);               //constructor will export the pin
	~GPIO();                        //destructor will unexport the pin

	int number() { return number_; }

	int         value(GPIO_VALUE v);
	GPIO_VALUE  value();

    private:
	int             number_;
	string          name_; 
    protected:
	int         export_gpio();
	int         unexport_gpio();
	int         gpio_write(string path,
                               string filename,
                               string value);
	int         gpio_write(string path, 
                               string filename, 
                               int    value);
	string      gpio_read (string path, 
                               string filename);
        string          path_;
};

class InputGPIO : public GPIO
{
    public:
        InputGPIO(int number);
        ~InputGPIO();

        int         edge_type(GPIO_EDGE);
        GPIO_EDGE   edge_type();
        void        debounce_time(int time){ debounce_time_ = time;}
        int         ev_count() { return ev_count_;}
        int         wait_for_edge(CallbackType callback);
        int         wait_for_edge();
    private:
        static void* threaded_poll(void* attr);
        int          debounce_time_;
        int          ev_count_; 
        pthread_t    thread_;
        bool         thread_running_;
        CallbackType callback_function_;
};

class OutputGPIO : public GPIO
{
    public:
        OutputGPIO(int number);
        ~OutputGPIO();

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
