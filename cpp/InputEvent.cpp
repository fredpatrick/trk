
#include "InputEvent.h"

#include <iostream>
#include <unistd.h>


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
