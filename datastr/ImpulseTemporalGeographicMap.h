#ifndef IMPULSE_TEMPORAL_GEOGRAPHIC_MAP_H
#define IMPULSE_TEMPORAL_GEOGRAPHIC_MAP_H

#include "TemporalGeographicMap.h"

namespace openworld {
  template<class T>
  class ImpulseTemporalGeographicMap : public PartialConfidenceTemporalGeographicMap<T> {
  protected:
    GeographicMap<T>* map;
    GeographicMap<T>* zero;

  public:
    ImpulseTemporalGeographicMap(DividedRange time, GeographicMap<T>* map)
      : PartialConfidenceTemporalGeographicMap<T>(NULL, NULL, time), map(map) {
      zero = new ConstantGeographicMap<T>(map->getLatitudes(), map->getLongitudes(), 0);
    }

    virtual GeographicMap<T>& operator[](Measure tt) {
      int index = this->getTimes().inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");
      
      if (index == 0)
        return *map;
      else
        return *zero;
    }
  };
}

#endif
