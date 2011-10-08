/******************************************************************************\
 * ConstantGeographicMap: Virtual map, with a constant level
 ******************************************************************************
\******************************************************************************/
#ifndef CONSTANT_GEOGRAPHIC_MAP_H
#define CONSTANT_GEOGRAPHIC_MAP_H

#include "GeographicMap.h"

using namespace std;

namespace openworld {
  template <class T>
  class ConstantGeographicMap : public GeographicMap<T> {
  protected:
    T value;

  public:
    ConstantGeographicMap(DividedRange latitudes, DividedRange longitudes, T value)
      : GeographicMap<T>(latitudes, longitudes) {
      this->value = value;
    }

    virtual double getDouble(Measure latitude, Measure longitude) const {
      return (double) value;
    }

    virtual T& getCell(unsigned rr, unsigned cc) {
      throw runtime_error("getCell on ConstantGeographicMap");
    }

    virtual T getCellConst(unsigned rr, unsigned cc) const {
      return value;
    }
  };
}

#endif
