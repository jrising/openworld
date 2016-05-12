#ifndef TEMPORAL_GEOGRAPHIC_MAP_H
#define TEMPORAL_GEOGRAPHIC_MAP_H

#include <ctime>
#include <vector>
#include "TimeSeries.h"
#include <measure/Inds.h>

namespace openworld {
  template<class T>
  class TemporalGeographicMap {
  protected:
    GeographicMap<T>* maps; // owns
    DividedRange time;

    /*void forget() {
      time = DividedRange(Inds::unixtime);
      maps = NULL;
      }*/

  public:
    TemporalGeographicMap(GeographicMap<T> maps[], DividedRange time)
      : time(time) {
      this->maps = maps;
    }

    virtual TemporalGeographicMap<T>* clone() {
      GeographicMap<T>* copies = NULL;
      if (maps) {
        vector< GeographicMap<T> > veccop;
        for (unsigned ii = 0; ii < time.count(); ii++)
          veccop.push_back(maps[ii]);
        copies = &veccop[0];
        return new TemporalGeographicMap<T>(copies, time);
      }

      return new TemporalGeographicMap<T>(maps, time);
    }

    virtual ~TemporalGeographicMap() {
      if (maps)
        delete[] maps;
    }

    virtual DividedRange getLongitudes() {
      if (maps)
        return maps->getLongitudes();
      // Should I throw exception here?
      return DividedRange(Inds::lon);
    }

    virtual DividedRange getLatitudes() {
      if (maps)
        return maps->getLatitudes();
      return DividedRange(Inds::lat);
    }

    DividedRange getTimes() {
      return time;
    }

    virtual GeographicMap<T>& operator[](Measure tt) {
      int index = time.inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");

      return maps[index];
    }

    virtual TimeSeries<T>* getTimeSeries(Measure latitude, Measure longitude) {
      TimeSeries<T>* result = new TimeSeries<T>(time);
      for (int index = 0; index <= time.count(); index++)
        result->get(index) = maps[index].getDouble(latitude, longitude);

      return result;
    }

    GeographicMap<T>* transfer() {
      GeographicMap<T>* data = maps;
      time = DividedRange(Inds::unixtime);
      maps = NULL;
      return data;
    }

    static TemporalGeographicMap<T>* loadDelimited(DividedRange latitudes, DividedRange longitudes, DividedRange time, string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      // According to IRI standard, data is ROWS*TIME x COLS
      unsigned int rrtt, cols;
      list<T>* lst = Matrix<T>::loadDelimited(filepath, &rrtt, &cols, parser, delimiter);

      // Construct all maps
      GeographicMap<T>* maps = new GeographicMap<T>[rrtt / latitudes.count()];
      typename list<T>::iterator iter = lst->begin();
      for (unsigned ii = 0; ii < rrtt / latitudes.count(); ii++) {
        maps[ii].initialize(latitudes, longitudes);
        maps[ii].loadIteratorInto(iter, latitudes.count(), longitudes.count());
      }

      return new TemporalGeographicMap<T>(maps, time);
    }
  };
}

#endif
