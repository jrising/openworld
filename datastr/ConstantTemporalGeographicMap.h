#ifndef CONSTANT_TEMPORAL_GEOGRAPHIC_MAP_H
#define CONSTANT_TEMPORAL_GEOGRAPHIC_MAP_H

#include "ConstantGeographicMap.h"
#include "TemporalGeographicMap.h"

namespace openworld {
  template<class T>
  class ConstantTemporalGeographicMap : public PartialConfidenceTemporalGeographicMap<T> {
  protected:
    GeographicMap<T>* map; // owns

  public:
    ConstantTemporalGeographicMap(DividedRange time, GeographicMap<T>* map)
      : PartialConfidenceTemporalGeographicMap<T>(NULL, NULL, time), map(map) {
    }

    ~ConstantTemporalGeographicMap() {
      delete map;
    }

    virtual GeographicMap<T>& operator[](Measure tt) {
      int index = this->getTimes().inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");
      
      return *map;
    }
  };
}

#endif
