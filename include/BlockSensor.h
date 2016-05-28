#ifndef TRK_BLOCKSENSOR_HH
#define TRK_BLOCKSENSOR_HH

#include "InputSensor.h"
#include "GPIO.h"
#include "GPIOConfig.h"

namespace trk {

class JobClock;

class BlockSensor : public InputSensor
{
    public:
        BlockSensor( int sensor_fd, int& n_event, const std::string& blk_name_);
        ~BlockSensor();

        void event (int ierr, InputGPIO* gpio);
        void    ignore_event();
        int     value();
        int     count();
        double  timeofday();

    private:
        int             sensor_fd_;
        int&            n_event_;
        bool            ignore_event_;
        std::string     blk_name_;

        int             value_;
        int             count_;
        double          tm_event_;

        JobClock*       job_clock_;
};

}

#endif
