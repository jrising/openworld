#include "Timer.h"

using namespace openworld;

map<string, clock_t> Timer::totals = map<string, clock_t>();
map<string, clock_t> Timer::starts = map<string, clock_t>();
bool Timer::initialized = false;
