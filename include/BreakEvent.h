#ifndef TRK_BREAKEVENT_HH
#define TRK_BREAKEVENT_HH

#include "InputEvent.h"

namespace trk {

class BreakEvent : public InputEvent
{
    public:
        BreakEvent(int sensor_fd);
        BreakEvent(double tm_event);

        ~BreakEvent();


        int write_event(int fd);
        void              print(int ntab);

    private:

};

}
#endif
