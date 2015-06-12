/******************************************************************************\
 * ScaledGeographicMap: A geographic map with delayed scaling
 ******************************************************************************
\******************************************************************************/
#ifndef SCALED_GEOGRAPHIC_MAP_H
#define SCALED_GEOGRAPHIC_MAP_H

#include "GeographicMap.h"

using namespace std;

#define OPTIMIZE_SCALECONVERT

namespace openworld {
  template <class T>
  class ScaledGeographicMap : public GeographicMap<T> {
  protected:
    GeographicMap<T>& source;
    T defval;

  public:
    ScaledGeographicMap(GeographicMap<T>& source, DividedRange latitudes, DividedRange longitudes, T defval)
      : GeographicMap<T>(latitudes, longitudes), source(source), defval(defval) {
    }

    virtual double getDouble(Measure latitude, Measure longitude) const {
      if (latitude < source.getLatitudes().getMin() || latitude >= source.getLatitudes().getMax() ||
          longitude < source.getLongitudes().getMin() || longitude >= source.getLongitudes().getMax())
        return defval;

      return source.getDouble(latitude, longitude);
    }

    #ifdef OPTIMIZE_SCALECONVERT
    bool convert(unsigned rr, unsigned cc, unsigned& new_rr, unsigned& new_cc, bool force = false) const {
      double lat = this->getLatitudes().min + this->getLatitudes().widths * (rr + .5);
      double lon = this->getLongitudes().min + this->getLongitudes().widths * (cc + .5);
      new_rr = source.getLatitudes().inRange(lat);
      new_cc = source.getLongitudes().inRange(lon);
      if (new_rr < 0 || new_rr >= source.getLatitudes().count() || new_cc < 0 || new_cc >= source.getLongitudes().count()) {
        if (force)
          throw runtime_error("convert: " + ToString::base10(rr) + ", " + ToString::base10(cc) + " -> " + ToString::out(lat) + ", " + ToString::out(lon));
        return false;
      }

      return true;
    }
    #else
    bool convert(unsigned rr, unsigned cc, unsigned& new_rr, unsigned& new_cc, bool force = false) const {
      Measure lat = this->getLatitudes().getMin() + this->getLatitudes().getWidths() * (rr + .5);
      Measure lon = this->getLongitudes().getMin() + this->getLongitudes().getWidths() * (cc + .5);
      new_rr = source.getLatitudes().inRange(lat);
      new_cc = source.getLongitudes().inRange(lon);
      if (new_rr < 0 || new_rr >= source.getLatitudes().count() || new_cc < 0 || new_cc >= source.getLongitudes().count()) {
        if (force)
          throw runtime_error("convert: " + ToString::base10(rr) + ", " + ToString::base10(cc) + " -> " + ToString::out(lat) + ", " + ToString::out(lon));
        return false;
      }

      return true;
    }
    #endif

    virtual T& getCell(unsigned rr, unsigned cc) {
      unsigned new_rr, new_cc;
      convert(rr, cc, new_rr, new_cc, true);
      return source.getCell(new_rr, new_cc);
    }
    
    virtual T getCellConst(unsigned rr, unsigned cc) const {
      unsigned new_rr, new_cc;
      if (!convert(rr, cc, new_rr, new_cc))
        return defval;
      return source.getCellConst(new_rr, new_cc);
    }
  };
}

#endif
