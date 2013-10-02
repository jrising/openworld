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

    DelayedPartialConfidenceTemporalGeographicMap(DataSupplier<GeographicMap<T>*>* supplier, double conf, DividedRange time)
      : PartialConfidenceTemporalGeographicMap<T>(NULL, NULL, time), helper(supplier) {
      for (unsigned ii = 0; ii < time.count(); ii++)
        this->confs[ii] = conf;
    }

    DelayedPartialConfidenceTemporalGeographicMap(DelayedGeographicMapHelper<T>& helper, double confs[], DividedRange time)
      : PartialConfidenceTemporalGeographicMap<T>(NULL, confs, time), helper(helper) {
    }

    virtual PartialConfidenceTemporalGeographicMap<T>* clone() {
      return new DelayedPartialConfidenceTemporalGeographicMap<T>(helper, this->confs, this->time);
    }

    virtual GeographicMap<T>& operator[](Measure tt) {
      int index = this->time.inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");

      return helper.getMap(index);
    }

    // debugging...
    GeographicMap<T>& getMap(unsigned index) {
      return helper.getMap(index);
    }

    static DelayedPartialConfidenceTemporalGeographicMap<T>* loadDelimited(DividedRange latitudes, DividedRange longitudes, DividedRange time, string filepath, string filepath_confs, T (*parser)(string) = NULL, char delimiter = ',', bool allowblank = true) {
      TimeSeries<double>* confs = TimeSeries<double>::loadDelimited(time, filepath_confs, parser, delimiter);
      // Using IRI standard ROW*TIME x COLS
      DelimitedSupplier<T>* supplier = new DelimitedSupplier<T>(filepath, parser, delimiter, allowblank);
      return new DelayedPartialConfidenceTemporalGeographicMap<T>(new RowTimeGeographicMapSupplier<T>(latitudes, longitudes, supplier),
                                                                  confs->transfer(), time);
    }

    template<class S>
    static DelayedPartialConfidenceTemporalGeographicMap<T>* convertScale(DelayedTemporalGeographicMap<S>* source, double scale) {
      ScaleSupplier<T, S>* supplier = new ScaleSupplier<T, S>(source->getPrimitiveDataSupplier(), scale);
      return new DelayedPartialConfidenceTemporalGeographicMap<T>(new RowTimeGeographicMapSupplier<T>(source->getLatitudes(), source->getLongitudes(), supplier), 1.0, source->getTimes());
    }

    template<class S>
    static DelayedPartialConfidenceTemporalGeographicMap<T>* convertSumScale(DelayedTemporalGeographicMap<S>* source1, DelayedTemporalGeographicMap<S>* source2, double scale) {
      ScaleSupplier<T, T>* supplier = new ScaleSupplier<T, T>(new SumSupplier<T, S>(source1->getPrimitiveDataSupplier(), source2->getPrimitiveDataSupplier()), scale);
      return new DelayedPartialConfidenceTemporalGeographicMap<T>(new RowTimeGeographicMapSupplier<T>(source1->getLatitudes(), source1->getLongitudes(), supplier), 1.0, source1->getTimes());
    }
  };
}

#endif
