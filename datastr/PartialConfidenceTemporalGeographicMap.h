#ifndef CONFIDENCE_TEMPORAL_GEOGRAPHIC_MAP_H
#define CONFIDENCE_TEMPORAL_GEOGRAPHIC_MAP_H

#include "TemporalGeographicMap.h"
#include "TimeSeries.h"

namespace openworld {
  template<class T>
  class PartialConfidenceTemporalGeographicMap : public TemporalGeographicMap<T> {
  protected:
    double* confs; // owns

    /*void forget() {
      TemporalGeographicMap<T>::forget();
      confs = NULL;
      }*/
    
  public:
    PartialConfidenceTemporalGeographicMap(GeographicMap<T> maps[], double confs[], DividedRange time)
      : TemporalGeographicMap<T>(maps, time) {
      this->confs = confs;
    }

    PartialConfidenceTemporalGeographicMap(TemporalGeographicMap<T> map, double conf = 1.0) 
      : TemporalGeographicMap<T>(map) {
      confs = new double[map.time.count()];
      for (int ii = 0; ii < map.time.count(); ii++)
        confs[ii] = conf;
    }
    
    ~PartialConfidenceTemporalGeographicMap() {
      if (confs)
        delete[] confs;
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
