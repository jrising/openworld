#ifndef D8_MAP_H
#define D8_MAP_H

#include <datastr/GeographicMap.h>

namespace openworld {
  class D8Map : public GeographicMap<double> {
  protected:
    int latsucc;

  public:
    template<class S>
    D8Map(const GeographicMap<S>& src, bool latincdown = false)
      : GeographicMap<double>(src) {

      if (latincdown)
        latsucc = -1;
      else
        latsucc = 1;
    }

    void getDirection(unsigned rr, unsigned cc, unsigned& rr1, unsigned& cc1) const {
      double dir = getCellConst(rr, cc);
          
      if (dir == 1) {
        rr1 = rr;
        cc1 = cc + 1;
      } else if (dir == 2) {
        rr1 = rr - latsucc;
        cc1 = cc + 1;
      } else if (dir == 3) {
        rr1 = rr - latsucc;
        cc1 = cc;
      } else if (dir == 4) {
        rr1 = rr - latsucc;
        cc1 = cc - 1;
      } else if (dir == 5) {
        rr1 = rr;
        cc1 = cc - 1;
      } else if (dir == 6) {
        rr1 = rr + latsucc;
        cc1 = cc - 1;
      } else if (dir == 7) {
        rr1 = rr + latsucc;
        cc1 = cc;
      } else {
        cc1 = cc + 1;
        rr1 = rr + latsucc;
      }
    }

    bool flowsInto(unsigned rr_from, unsigned cc_from, unsigned rr_to, unsigned cc_to) {
      unsigned rr1, cc1;

      getDirection(rr_from, cc_from, rr1, cc1);
      return (rr1 == rr_to && cc1 == cc_to);
    }

    void getDirection(Measure lat, Measure lon, Measure& lat1, Measure& lon1) const {
      unsigned rr1, cc1;
      getDirection(getLatitudes().inRange(lat), getLongitudes().inRange(lon), rr1, cc1);
      
      lat1 = getLatitudes().getCellCenter(rr1);
      lon1 = getLongitudes().getCellCenter(cc1);
    }
  };
}

#endif
