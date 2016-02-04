

#include <iostream>
#include <unistd.h>
#include "GPIO.h"
#include "EnablePCB.h"

using namespace std;
using namespace trk;

int main() {

    cout << "BeagleBoneBlack driver for exercising trk cicuits" <<endl;

    EnablePCB* pcp = EnablePCB::instance();
    cout << "Power controler created " << endl;


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



