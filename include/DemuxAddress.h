#ifndef TRK_DEMUXADDRESS_HH
#define TRK_DEMUXADDRESS_HH

#include <string>
#include<bitset>
#include "GPIOConfig.h"
#include "GPIO.h"

namespace trk {

class DemuxAddress {

    public:
        static DemuxAddress* instance();

        ~DemuxAddress();

        bool set (int sw_num, SW_DIRECTION& sw_direc);

    protected:
        DemuxAddress();

    private:
        GPIO*       gpios_[5];
        static      DemuxAddress*   instance_;
};

}

#endif
