#ifndef TRK_TRACKEVENT_HH
#define TRK_TRACKEVENT_HH

#include "InputEvent.h"
#include "trkutl.h"

namespace trk {

class TrackEvent : public InputEvent
{
    public:
        TrackEvent(int sensor_fd);
        TrackEvent(double tm_event,
                   const std::string& zone_name,
                   const TRK_STATE&   track_state);
        ~TrackEvent();

        int         write_event(int fd);
        void        print(int ntab);
        std::string zone_name();
        TRK_STATE   track_state();
    private:
        std::string zone_name_;
        TRK_STATE   track_state_;

};

}
#endif
