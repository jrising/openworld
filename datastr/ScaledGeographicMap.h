/******************************************************************************\
 * ScaledGeographicMap: A geographic map with delayed scaling
 ******************************************************************************
\******************************************************************************/
#ifndef SCALED_GEOGRAPHIC_MAP_H
#define SCALED_GEOGRAPHIC_MAP_H

#include "GeographicMap.h"

using namespace std;

namespace openworld {
  template <class T>
  class ScaledGeographicMap : public GeographicMap<T> {
  protected:
    GeographicMap<T>& source;

  public:
    ScaledGeographicMap(GeographicMap<T>& source, DividedRange latitudes, DividedRange longitudes)
      : GeographicMap<T>(latitudes, longitudes), source(source) {
    }

    virtual double getDouble(Measure latitude, Measure longitude) const {
      return source.getDouble(latitude, longitude);
    }

    virtual T& getCell(unsigned rr, unsigned cc) {
      return source.getCell(source.getLatitudes().inRange(this->getLatitudes().getMin() + this->getLatitudes().getWidths() * rr),
                            source.getLongitudes().inRange(this->getLongitudes().getMin() + this->getLongitudes().getWidths() * cc));
    }
    
    virtual T getCellConst(unsigned rr, unsigned cc) const {
      return source.getCellConst(source.getLatitudes().inRange(this->getLatitudes().getMin() + this->getLatitudes().getWidths() * rr),
                                 source.getLongitudes().inRange(this->getLongitudes().getMin() + this->getLongitudes().getWidths() * cc));
    }
  };
}

#endif
