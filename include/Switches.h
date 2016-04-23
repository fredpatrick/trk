#ifndef TRK_SWITCHES_HH
#define TRK_SWITCHES_HH

#include "Switch.h"

namespace trk {

class Switches {
    public:
        Switches();
        ~Switches();

        void list_state();
    private:
        Switch*     switch_[6];
};

}

#endif
