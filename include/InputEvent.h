#ifndef TRK_INPUTEVENT_HH
#define TRK_INPUTEVENT_HH

#include <string>

namespace trk {

class InputEvent 
{
    public:
        InputEvent();
        ~InputEvent();

        std::string     tag();
        virtual int     write_event(int fd);
        virtual void    print(int ntab);
        double          tm_event();
    protected:
        std::string     tag_;
        double          tm_event_;
    private:
};
}

#endif
