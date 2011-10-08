/******************************************************************************\
 * Indicator: a class of measurements; indicators source of frame of reference
 ******************************************************************************
 *
\******************************************************************************/

#ifndef INDICATOR_H
#define INDICATOR_H

#include <string>
#include <iostream>
#include "../dims/Dimensions.h"

using namespace std;

namespace openworld {
  class Indicator {
  protected:
    string name;
    Dimensions dims;
		
  public:
    Indicator(string name, Dimensions dims) 
      : dims(dims) {
      this->name = name;
    }
    
    friend ostream& operator<<(ostream& out, const Indicator& xx) {
      return out << xx.name << " [" << xx.dims << "]";
    }
  
    string getName() {
      return name;
    }
		
    Dimensions getDimensions() const {
      return dims;
    }

    bool operator==(const Indicator& b) const {
      return (this == &b || (name == b.name && dims == b.dims));
    }

    bool operator!=(const Indicator& b) const {
      return !(*this == b);
    }

    // Mathematics
		
    friend Indicator operator-(const Indicator& a) {
      return Indicator("-" + a.name, a.dims);
    }

    Indicator operator+(const Indicator& b) const {
      if (dims != b.dims)
        throw runtime_error("dimensions mismatch to +");
      return Indicator("(" + name + " + " + b.name + ")", dims);
    }
		
    Indicator operator-(const Indicator& b) const {
      if (dims != b.dims)
        throw runtime_error("dimensions mismatch to -");
      return Indicator("(" + name + " - " + b.name + ")", dims);
    }
		
    Indicator operator*(const Indicator& b) const {
      return Indicator(name + " " + b.name, dims * b.dims);
    }
  
    Indicator operator/(const Indicator& b) const {
      return Indicator("(" + name + ")/(" + b.name + ")", dims / b.dims);
    }
  };
}

#endif
