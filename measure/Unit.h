/******************************************************************************\
 * Unit: a class of measurements
 ******************************************************************************
 *
\******************************************************************************/

#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <iostream>
#include <stdexcept>
#include "../dims/Dimensions.h"

using namespace std;

namespace openworld {
  class Unit {
  protected:
    string name;
    Dimensions dims;
		
  public:
    Unit(string name, Dimensions dims)
      : dims(dims) {
      this->name = name;
    }
    
    string toString() {
      return name + " [" + dims.toString() + "]";
    }

    friend ostream& operator<<(ostream& out, const Unit& xx) {
      return out << xx.name << " [" << xx.dims << "]";
    }
  
    string getName() {
      return name;
    }
		
    Dimensions getDimensions() const {
      return dims;
    }

    bool operator==(const Unit& b) const {
      return (this == &b || (name == b.name && dims == b.dims));
    }

    bool operator!=(const Unit& b) const {
      return !(*this == b);
    }

    // Mathematics
		
    friend Unit operator-(const Unit& a) {
      return Unit("-" + a.name, a.dims);
    }

    Unit operator+(const Unit& b) const {
      if (dims != b.dims)
        throw runtime_error("dimensions mismatch to +");
      return Unit("(" + name + " + " + b.name + ")", dims);
    }
		
    Unit operator-(const Unit& b) const {
      if (dims != b.dims)
        throw runtime_error("dimensions mismatch to -");
      return Unit("(" + name + " - " + b.name + ")", dims);
    }
		
    Unit operator*(const Unit& b) const {
      return Unit(name + " " + b.name, dims * b.dims);
    }
  
    Unit operator/(const Unit& b) const {
      return Unit("(" + name + ")/(" + b.name + ")", dims / b.dims);
    }

    // Serialization

    ostream& streamInsert(ostream& os) const {
      return dims.streamInsert(os) << name << endl;
    }

    static Unit streamExtract(istream& in) {
      Dimensions dims = Dimensions::streamExtract(in);
      string name;
      getline(in, name);

      return Unit(name, dims);
    }
  };
}

#endif
