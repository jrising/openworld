#ifndef DIMS_H
#define DIMS_H

#include <math.h>
#include "GlobalDimensions.h"

namespace openworld {
  class Dims {
  public:
    static Dimensions& s;
    /*static Dimensions min = GlobalDimensions::add("min", Dims::s*60);
  static Dimensions hr = GlobalDimensions::add("hr", min*60);
  static Dimensions day = GlobalDimensions::add("day", hr*24);
  //static Dimensions month = GlobalDimensions::add("month", Depends(theDate, day*365.25/12));
  //static Dimensions year = GlobalDimensions::add("year", Depends(theDate, day*365.25));*/
  
  // space variables
    static Dimensions& m;
  /*static Dimensions cm = GlobalDimensions::add("cm", m/100);
  static Dimensions mm = GlobalDimensions::add("mm", m/1000);
  static Dimensions km = GlobalDimensions::add("km", m*1000);
  static Dimensions inch = GlobalDimensions::add("in", cm*2.54);
  static IDisposable ft = GlobalDimensions::add("ft", inch*12);
  static IDisposable mi = GlobalDimensions::add("mi", ft*5280);*/
		
  // mass variables
    static Dimensions& kg;

  // angles
  static Dimensions& deg;
  /*static Dimensions rad = GlobalDimensions::get("rad");
  static Dimensions deg = GlobalDimensions::add("deg", rad * 180.0 / M_PI);*/
  };
}

#endif
