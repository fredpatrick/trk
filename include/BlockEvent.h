#ifndef TRK_BLOCKEVENT_HH
#define TRK_BLOCKEVENT_HH

#include "InputEvent.h"

namespace trk {
class BlockEvent : public InputEvent
{
    public: 
        BlockEvent(int sensor_fd);
        BlockEvent(double             tm_event,
                   const std::string& block_name,
                   const std::string& block_status);
        ~BlockEvent();

        int             write_event(int fd);
        void            print(int ntab);
        std::string     block_name();
        std::string     block_status();

    private:
        std::string         block_name_;
        std::string         block_status_;
};

}
#endif
