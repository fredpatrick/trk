
#include <iostream>
#include <unistd.h>
#include "GPIO.h"

using namespace std;
using namespace trk;

int
received_event(int ier, void* ptr)
{
    cout << "received_event, ier = " << ier << endl;
    InputGPIO* gpio = static_cast<InputGPIO*>(ptr);
    int gpio_number = gpio->number();
    cout << "received_event, gpio number = " << gpio_number << endl;
    int value = (int) gpio->value();
    int count = gpio->ev_count();
    cout << "received_event,value = " << value << ", count = " << count << endl;
}

int main()
{
    cout << "chkSwitchInput " << endl;

    InputGPIO* gpio_thru = new InputGPIO(79);
    InputGPIO* gpio_out  = new InputGPIO(78);

    gpio_thru->edge_type(BOTH);
    gpio_thru->debounce_time(200);
    gpio_thru->wait_for_edge(&received_event);
    cout << "chkSwitchInput, Poll started on 79, thru position" << endl;

    gpio_out->edge_type(BOTH);
    gpio_out->debounce_time(200);
    gpio_out->wait_for_edge(&received_event);
    cout << "chkSwitchInput, Poll started on 78, out position" << endl;

    usleep(100000000);
    cout << "chkSwitchInput, finished sleeping for 100 seconds" << endl;

    delete gpio_thru;
    delete gpio_out;

    return 0;
}
