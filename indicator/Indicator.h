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
    double min;
    double max;
		
  public:
    Indicator(string name, Dimensions dims, double min, double max)
      : dims(dims) {
      this->name = name;
      this->min = min;
      this->max = max;
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

    double getMin() const {
      return min;
    }

    double getMax() const {
      return max;
    }

    bool operator==(const Indicator& b) const {
      return (this == &b || (name == b.name && dims == b.dims));
    }

    bool operator!=(const Indicator& b) const {
      return !(*this == b);
    }

    double getRandomValue() {
      double f = (double) rand() / RAND_MAX;
      return f*(max - min) + min;
    }

    // Mathematics
		
    friend Indicator operator-(const Indicator& a) {
      return Indicator("-" + a.name, a.dims, -a.max, -a.min);
    }

    Indicator operator+(const Indicator& b) const {
      if (dims != b.dims)
        throw runtime_error("dimensions mismatch to +");
      return Indicator("(" + name + " + " + b.name + ")", dims, min + b.min, max + b.max);
    }
		
    Indicator operator-(const Indicator& b) const {
      if (dims != b.dims)
        throw runtime_error("dimensions mismatch to -");
      return Indicator("(" + name + " - " + b.name + ")", dims, min - b.min, max - b.max);
    }
		
    Indicator operator*(const Indicator& b) const {
      double newmin = std::min(min * b.min, std::min(min * b.max, b.min * max));
      double newmax = std::max(max * b.max, min * b.min);
      return Indicator(name + " " + b.name, dims * b.dims, newmin, newmax);
    }
  
    Indicator operator/(const Indicator& b) const {
      double newmin = std::min(min / b.min, std::min(min / b.max, b.min / max));
      double newmax = std::max(max / b.max, min / b.min);
      return Indicator("(" + name + ")/(" + b.name + ")", dims / b.dims, newmin, newmax);
    }

    // Serialization

    ostream& streamInsert(ostream& os) const {
      return dims.streamInsert(os) << name << endl << min << " " << max << " ";
    }

    static Indicator streamExtract(istream& in) {
      Dimensions dims = Dimensions::streamExtract(in);
      string name;
      getline(in, name);

      double min, max;
      in >> min >> max;

      return Indicator(name, dims, min, max);
    }
  };
}

#endif
