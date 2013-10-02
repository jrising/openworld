#ifndef UNITS_H
#define UNITS_H

#include <math.h>
#include "Unit.h"

namespace openworld {
  class Units {
  public:
    static Unit none;
    static Unit individuals;

    static Unit s;
    static Unit yr;
    /*static Unit min = GlobalUnit::add("min", Dims::s*60);
  static Unit hr = GlobalUnit::add("hr", min*60);
  static Unit day = GlobalUnit::add("day", hr*24);
  //static Unit month = GlobalUnit::add("month", Depends(theDate, day*365.25/12));
  //static Unit year = GlobalUnit::add("year", Depends(theDate, day*365.25));*/
  
  // space variables
    static Unit m;
  /*static Unit cm = GlobalUnit::add("cm", m/100);
  static Unit mm = GlobalUnit::add("mm", m/1000);
  static Unit km = GlobalUnit::add("km", m*1000);
  static Unit inch = GlobalUnit::add("in", cm*2.54);
  static IDisposable ft = GlobalUnit::add("ft", inch*12);
  static IDisposable mi = GlobalUnit::add("mi", ft*5280);*/

    static Unit ha;
		
  // mass variables
    static Unit kg;
    static Unit mt;

  // angles
    static Unit deg;
    /*static Unit rad = GlobalUnit::get("rad");
      static Unit deg = GlobalUnit::add("deg", rad * 180.0 / M_PI);*/
  };
}

#endif
