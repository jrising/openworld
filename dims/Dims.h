#ifndef DIMS_H
#define DIMS_H

#include <math.h>
#include "Dimensionless.h"
#include "GlobalDimensions.h"

namespace openworld {
  class Dims {
  public:
    static Dimensions& none() {
      return Dimensionless::getInstance();
    }

    static Dimensions& individuals() {
      static Dimensions& sing = GlobalDimensions::get("individuals");
      return sing;
    }

    static Dimensions& time() {
      static Dimensions& sing = GlobalDimensions::get("time");
      return sing;
    }
  
    static Dimensions& distance() {
      static Dimensions& sing = GlobalDimensions::get("distance");
      return sing;
    }

    static Dimensions& mass() {
      static Dimensions& sing = GlobalDimensions::get("mass");
      return sing;
    }

    static Dimensions& angle() {
      static Dimensions& sing = GlobalDimensions::get("angle");
      return sing;
    }
  };
}

#endif
