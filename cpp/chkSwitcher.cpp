

#include <iostream>
#include <unistd.h>
#include "GPIO.h"
#include "switcher.h"

using namespace std;
using namespace trk;

int main() {

    cout << "BeagleBoneBlack switch driver test" <<endl;

    GPIO*   gpios[5];
    gpios[0] = new OutputGPIO(65);
    gpios[1] = new OutputGPIO(27);
    gpios[2] = new OutputGPIO(46);
    gpios[3] = new OutputGPIO(47);
    gpios[4] = new OutputGPIO(26);

    cout << "GPIOs created " << endl;

    trk::Switcher* switcher = new trk::Switcher(gpios);

    cout << "Switcher initialized" << endl;

    string yesno;
    int switch_index;
    int pos;
    bool done = false;
    while ( done == false ) {

        cout << "Enter switch_index (0:2), pos (0:1) " << endl;
        cin >> switch_index >> pos;
         switcher->flip_switch(switch_index, pos);

        cout << "Switch set " << endl;
        cout << "Continue? (y/n)" << endl;
        cin >> yesno;
        switcher->reset();
        if ( yesno == "n" ) done = true;
    }
    delete gpios[0];
    delete gpios[1];
    delete gpios[2];
    delete gpios[3];
    delete gpios[4];
    return 0;
}



