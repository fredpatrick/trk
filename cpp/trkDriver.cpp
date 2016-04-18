

#include <iostream>
#include <unistd.h>
#include "GPIO.h"
#include "EnablePCB.h"

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

int main() {

    cout << "BeagleBoneBlack driver for exercising trk cicuits" <<endl;

    EnablePCB* pcp = EnablePCB::instance();
    cout << "Power controler created " << endl;


    InputGPIO* gpio_thru = new InputGPIO(79);

    gpio_thru->edge_type(BOTH);
    gpio_thru->debounce_time(200);
    gpio_thru->wait_for_edge(&received_event);
    cout << "trkDriver, Poll started on 79, thru position" << endl;

    usleep(100000000);
    cout << "trkDriver, finished sleeping for 100 seconds" << endl;

    delete gpio_thru;

    string yesno;
    bool done = false;
    while ( done == false ) {

        cout << "Turn power on/off? (on/off/done)" << endl;
        cin >> yesno;
        if ( yesno == "on" ) {
            pcp->on();
        } else if (yesno == "off") {
            pcp->off();
        } else if (yesno == "done") {
            done = true;
        }
    }
    delete pcp;
    return 0;
}

