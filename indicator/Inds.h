#ifndef INDS_H
#define INDS_H

#include <stdexcept>
#include "Indicator.h"
#include "../dims/Dims.h"

using namespace std;

namespace openworld {
  class Inds {
  public:
    static Indicator lat;
    static Indicator lon;
    
    static Indicator unixtime;
  };
}

#endif
