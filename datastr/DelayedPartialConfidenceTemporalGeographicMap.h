#ifndef DELAYED_CONFIDENCE_TEMPORAL_GEOGRAPHIC_MAP_H
#define DELAYED_CONFIDENCE_TEMPORAL_GEOGRAPHIC_MAP_H

#include "TemporalGeographicMap.h"
#include "TimeSeries.h"

namespace openworld {
  template<class T>
  class DelayedPartialConfidenceTemporalGeographicMap : public PartialConfidenceTemporalGeographicMap<T> {
  protected:
    DelayedGeographicMapHelper<T> helper;

  public:
    DelayedPartialConfidenceTemporalGeographicMap(DataSupplier<GeographicMap<T>*>* supplier, double confs[], DividedRange time)
      : PartialConfidenceTemporalGeographicMap<T>(NULL, confs, time), helper(supplier) {
    }

    virtual GeographicMap<T>& operator[](Measure tt) {
      int index = this->time.inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");

      return helper.getMap(index);
    }

    static DelayedPartialConfidenceTemporalGeographicMap<T>* loadDelimited(DividedRange latitudes, DividedRange longitudes, DividedRange time, string filepath, string filepath_confs, T (*parser)(string) = NULL, char delimiter = ',') {
      TimeSeries<double>* confs = TimeSeries<double>::loadDelimited(time, filepath_confs, parser, delimiter);
      // Using IRI standard ROW*TIME x COLS
      DelimitedSupplier<T>* supplier = new DelimitedSupplier<T>(filepath, parser, delimiter);
      return new DelayedPartialConfidenceTemporalGeographicMap<T>(new RowTimeGeographicMapSupplier<T>(latitudes, longitudes, supplier),
                                                                  confs->transfer(), time);
    }
  };
}

#endif
