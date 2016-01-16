

#ifndef TRK_SWITCHER_HH
#define TRK_SWITCHER_HH

#include "switch.h"
#include "GPIO.h"

namespace trk {

class Switcher {

    public:
        Switcher(GPIO** gpios);
        ~Switcher();

        void flip_switch(int switch_index,
                         int pos);

        void reset();
    private:
        GPIO* gpio_a0;
        GPIO* gpio_a1;
        GPIO* gpio_a2;
        GPIO* gpio_a3;
        GPIO* gpio_a4;
};

}

#endif /*TRK_SWITCHER_HH */
