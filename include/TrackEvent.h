#ifndef TRK_TRACKEVENT_HH
#define TRK_TRACKEVENT_HH

#include "InputEvent.h"

namespace trk {

class TrackEvent : public InputEvent
{
    public:
        TrackEvent(int sensor_fd);
        TrackEvent(double tm_event,
                   const std::string& zone_name,
                   const std::string& type);
        ~TrackEvent();

        int         write_event(int fd);
        void        print(int ntab);
        std::string zone_name();
        std::string type();
    private:
        std::string zone_name_;
        std::string type_;

};

}
#endif
