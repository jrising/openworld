#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <map>

using namespace std;

namespace openworld {
  class Timer {
  protected:
    static map<string, clock_t> totals;
    static map<string, clock_t> starts;
    static bool initialized;

  public:
    static void start(string name) {
      if (!initialized) {
        totals = map<string, clock_t>();
        starts = map<string, clock_t>();
        initialized = true;
      }

      starts[name] = clock();
    }

    static void end(string name) {
      totals[name] += clock() - starts[name];
    }

    static time_t get(string name) {
      return totals[name];
    }
  };
}

#endif
