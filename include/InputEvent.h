#ifndef TRK_INPUTEVENT_HH
#define TRK_INPUTEVENT_HH

#include <string>

namespace trk {

class InputEvent 
{
    public:
        InputEvent();
        ~InputEvent();

        virtual std::string     tag();
        virtual int     write_event(int fd) = 0;
        virtual void    print(int ntab) = 0;
        virtual double  tm_event();
        virtual int     event_seq_n();
    protected:
        std::string     tag_;
        double          tm_event_;

        static int      event_seq_n_;
    private:
};
}

#endif
