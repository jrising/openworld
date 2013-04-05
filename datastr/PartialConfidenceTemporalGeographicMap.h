#ifndef CONFIDENCE_TEMPORAL_GEOGRAPHIC_MAP_H
#define CONFIDENCE_TEMPORAL_GEOGRAPHIC_MAP_H

#include "TemporalGeographicMap.h"
#include "TimeSeries.h"

namespace openworld {
  template<class T>
  class PartialConfidenceTemporalGeographicMap : public TemporalGeographicMap<T> {
  protected:
    double* confs; // owns

    PartialConfidenceTemporalGeographicMap(DividedRange time) 
      : TemporalGeographicMap<T>(NULL, time) {
      confs = NULL;
    }
    
  public:
    PartialConfidenceTemporalGeographicMap(GeographicMap<T> maps[], double confs[], DividedRange time)
      : TemporalGeographicMap<T>(maps, time) {
      this->confs = new double[time.count()];
      for (unsigned ii = 0; ii < time.count(); ii++)
        this->confs[ii] = confs[ii];
    }

    PartialConfidenceTemporalGeographicMap(TemporalGeographicMap<T> map, double conf = 1.0) 
      : TemporalGeographicMap<T>(map) {
      confs = new double[map.time.count()];
      for (int ii = 0; ii < map.time.count(); ii++)
        confs[ii] = conf;
    }
    
    virtual ~PartialConfidenceTemporalGeographicMap() {
      if (confs)
        delete[] confs;
    }

    virtual PartialConfidenceTemporalGeographicMap<T>* clone() {
      PartialConfidenceTemporalGeographicMap<T>* copy = new PartialConfidenceTemporalGeographicMap<T>(this->time);
      if (confs) {
        copy->confs = new double[this->time.count()];
        for (unsigned ii = 0; ii < this->time.count(); ii++)
          copy->confs[ii] = confs[ii];
      }

      return copy;
    }

    static PartialConfidenceTemporalGeographicMap<T>* loadDelimited(DividedRange latitudes, DividedRange longitudes, DividedRange time, string filepath, string filepath_confs, T (*parser)(string) = NULL, char delimiter = ',') {
      TemporalGeographicMap<T>* data = TemporalGeographicMap<T>::loadDelimited(latitudes, longitudes, time, filepath, parser, delimiter);
      TimeSeries<double>* confs = TimeSeries<double>::loadDelimited(time, filepath_confs, parser, delimiter);

      PartialConfidenceTemporalGeographicMap<T>* result = new PartialConfidenceTemporalGeographicMap<T>(data->transfer(), confs->transfer(), time);

      return result;
    }
  };
}

#endif
