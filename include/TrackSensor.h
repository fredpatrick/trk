#ifndef TRK_TRACKSENSOR_HH
#define TRK_TRACKSENSOR_HH

#include "InputSensor.h"
#include "GPIO.h"
#include "GPIOConfig.h"

namespace trk {

class JobClock;

class TrackSensor : public InputSensor
{
    public:
        TrackSensor( int sensor_fd, int& n_event, const std::string& zone_name_);
        ~TrackSensor();

        void event (int ierr, InputGPIO* gpio);
        void    ignore_event(bool s);
        int     value();
        int     count();
        double  timeofday();

    private:
        int             sensor_fd_;
        int&            n_event_;
        bool            ignore_event_;
        std::string     zone_name_;

        int             value_;
        int             count_;
        double          tm_event_;

        JobClock*       job_clock_;
};

}

#endif
