#include "InputSensor.h"

pthread_mutex_t trk::InputSensor::write_event_ = PTHREAD_MUTEX_INITIALIZER;

trk::InputSensor::
InputSensor()
{
}

trk::InputSensor::
~InputSensor()
{
}
