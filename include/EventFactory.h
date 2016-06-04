#ifndef TRK_EVENTFACTORY_HH
#define TRK_EVENTFACTORY_HH

#include <iostream>

namespace trk {

class InputEvent;

class EventFactory {
    public:
        static EventFactory* instance();

        ~EventFactory();

        InputEvent* get_next_event( int fd);

    protected:
        EventFactory();

    private:
        static EventFactory* instance_;
};

}
#endif
