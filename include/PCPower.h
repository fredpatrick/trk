
#ifndef TRK_PCPOWER_HH
#define TRK_PCPOWER_HH

#include "GPIO.h"

namespace trk {

class PCPower {

    public:
        static PCPower* instance();

        ~PCPower();

        int on();
        int off();

    protected:
        PCPower();

    private:
        OutputGPIO* gpio_pwr_;
        static PCPower*    instance_;
};


}

#endif

