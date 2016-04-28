#ifndef TRK_JOBCLOCK_HH
#define TRK_JOBCLOCK_HH

#include <iostream>
#include <sys/time.h>


namespace trk {

class JobClock {
    public:
        static JobClock* instance();

        ~JobClock();

        double job_time();


    protected:
        JobClock();

    private:
        double  t0_;

        static JobClock*    instance_;
};

}

std::ostream& 
operator<<(std::ostream&, trk::JobClock&);


#endif
