#ifndef TRK_BLOCKEVENT_HH
#define TRK_BLOCKEVENT_HH

#include "InputEvent.h"
#include "trkutl.h"

namespace trk {
class BlockEvent : public InputEvent
{
    public: 
        BlockEvent(int sensor_fd);
        BlockEvent(double             tm_event,
                   const std::string& block_name,
                   const BLK_STATE&   block_state);
        ~BlockEvent();

        int             write_event(int fd);
        void            print(int ntab);
        std::string     block_name();
        BLK_STATE       block_state();

    private:
        std::string         block_name_;
        BLK_STATE           block_state_;
};

}
#endif
