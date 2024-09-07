#include "Tracker.h"

std::unique_ptr<NoOpTracker> NoOpTracker::_instance;
std::once_flag NoOpTracker::_initFlag;