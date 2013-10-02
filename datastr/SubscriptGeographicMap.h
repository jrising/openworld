/******************************************************************************\
 * SubscriptGeographicMap: Virtual map, describing a logical subset
 ******************************************************************************
\******************************************************************************/
#ifndef SUBSCRIPT_GEOGRAPHIC_MAP_H
#define SUBSCRIPT_GEOGRAPHIC_MAP_H

#include "GeographicMap.h"

using namespace std;

namespace openworld {
  template <class T>
  class SubscriptGeographicMap : public GeographicMap<T> {
  protected:
    GeographicMap<T>& source;
    GeographicMap<bool>& logical;

  public:
  SubscriptGeographicMap(GeographicMap<T>& source, GeographicMap<bool>& logical)
    : GeographicMap<T>(source.getLatitudes(), source.getLongitudes()), source(source), logical(logical) {
    }

    virtual double getDouble(Measure latitude, Measure longitude) const {
      if (logical.getDouble(latitude, longitude))
        return source.getDouble(latitude, longitude);

      return nan("");
    }

    virtual T& getCell(unsigned rr, unsigned cc) {
      if (logical.getCellConst(rr, cc))
        return source.getCell(rr, cc);

      throw runtime_error("getCell out of subset");
    }

    virtual T getCellConst(unsigned rr, unsigned cc) const {
      if (logical.getCellConst(rr, cc))
        return source.getCellConst(rr, cc);

      throw runtime_error("getCellConst out of subset");
    }

    virtual GeographicMap<T>& operator=(const T value) {
      for (unsigned rr = 0; rr < source.getLatitudes().count(); rr++)
        for (unsigned cc = 0; cc < source.getLongitudes().count(); cc++)
          if (logical.getCellConst(rr, cc))
            source.getCell(rr, cc) = value;
    }
  };
}

#endif
