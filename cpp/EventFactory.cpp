
#include "EventFactory.h"
#include "BreakEvent.h"
#include "BlockEvent.h"
#include "SwitchEvent.h"
#include "TrackEvent.h"

trk::EventFactory* trk::EventFactory::instance_ = 0;

trk::EventFactory*
trk::EventFactory::
instance()
{
    if ( !instance_ ) {
        instance_ = new EventFactory();
    }
    return instance_;
}

trk::EventFactory::
EventFactory()
{
}

trk::EventFactory::
~EventFactory()
{
}

trk::InputEvent*
trk::EventFactory::
get_next_event(int fd)
{
    InputEvent* event;

    char ctag[4];
    read(fd, ctag,4);
    std::string tag = ctag;

    if        ( tag == "BRK" ) {
        event = new BreakEvent(fd);
    } else if ( tag == "BLK" ) {
        event = new BlockEvent(fd);
    } else if ( tag == "SW " ) {
        event = new SwitchEvent(fd);
    } else if ( tag == "TRK" ) {
        event = new TrackEvent(fd);
    } else {
        return 0;
    }
    return event;
}

