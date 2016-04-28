
#include "JobClock.h"

trk::JobClock* trk::JobClock::instance_ = 0;

trk::JobClock*
trk::
JobClock::instance()
{
    if( !instance_ ) {
        instance_ = new JobClock();
    }
    return instance_;
}

trk::JobClock::
JobClock()
{
    timeval t;
    gettimeofday(&t, NULL);

    t0_ = t.tv_sec + t.tv_usec * 1.0e-06;
    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    std::cout << "JobClock.ctor, t0_ = " << t0_ << std::endl;
}

trk::JobClock::
~JobClock()
{
}

double
trk::JobClock::
job_time()
{
    timeval t;
    gettimeofday(&t, NULL);

    double tm = t.tv_sec + t.tv_usec * 1.0e-06;
    return tm - t0_;
}

std::ostream&
operator<<(std::ostream& ostrm, trk::JobClock& job_clock)
{
    ostrm << job_clock.job_time();
    return ostrm;
}

