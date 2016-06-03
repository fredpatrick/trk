#ifndef TRK_SWITCHEVENT_HH
#define TRK_SWITCHEVENT_HH

#include "InputEvent.h"
#include "trkutl.h"

namespace trk {

class SwitchEvent : public InputEvent
{
    public:
        SwitchEvent(int          sensor_fd);
        SwitchEvent(double       tm_event,
                    int          sw_num,
                    SW_DIRECTION sw_direc);
        ~SwitchEvent();

        int          write_event(int fd);
        void         print(int ntab);

        int          sw_num();
        SW_DIRECTION sw_direc();
    private:
        int          sw_num_;
        SW_DIRECTION sw_direc_;
};

}

#endif
