
#include "InputEvent.h"

#include <iostream>
#include <unistd.h>

int trk::InputEvent::event_seq_n_ = 0;

trk::InputEvent::
InputEvent()
{
}

trk::InputEvent::
~InputEvent()
{
}

std::string
trk::InputEvent::
tag()
{
    return tag_;
}

double
trk::InputEvent::
tm_event()
{
    return tm_event_;
}

int
trk::InputEvent::
event_seq_n()
{
    return event_seq_n_;
}
