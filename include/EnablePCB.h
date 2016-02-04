
#ifndef TRK_ENABLEPCB_HH
#define TRK_ENABLEPCB_HH

#include "GPIO.h"

namespace trk {

class EnablePCB {

    public:
        static EnablePCB* instance();

        ~EnablePCB();

        int on();
        int off();

    protected:
        EnablePCB();

    private:
        GPIO*                 gpio_pwr_;
        static EnablePCB*     instance_;
};


}

#endif

