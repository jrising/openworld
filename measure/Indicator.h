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
#include "Unit.h"

using namespace std;

namespace openworld {
  class Indicator {
  protected:
    string name;
    Unit unit;
    double min;
    double max;
		
  public:
    Indicator(string name, Unit unit, double min, double max)
      : unit(unit) {
      this->name = name;
      this->min = min;
      this->max = max;
    }
    
    friend ostream& operator<<(ostream& out, const Indicator& xx) {
      return out << xx.name << " [" << xx.unit << "]";
    }
  
    string getName() {
      return name;
    }
		
    Unit getUnit() const {
      return unit;
    }

    double getMin() const {
      return min;
    }

    double getMax() const {
      return max;
    }

    bool operator==(const Indicator& b) const {
      return (this == &b || (name == b.name && unit == b.unit));
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
      return Indicator("-" + a.name, a.unit, -a.max, -a.min);
    }

    Indicator operator+(const Indicator& b) const {
      if (unit != b.unit)
        throw runtime_error("unit mismatch to +");
      return Indicator("(" + name + " + " + b.name + ")", unit, min + b.min, max + b.max);
    }
		
    Indicator operator-(const Indicator& b) const {
      if (unit != b.unit)
        throw runtime_error("unit mismatch to -");
      return Indicator("(" + name + " - " + b.name + ")", unit, min - b.min, max - b.max);
    }
		
    Indicator operator*(const Indicator& b) const {
      double newmin = std::min(min * b.min, std::min(min * b.max, b.min * max));
      double newmax = std::max(max * b.max, min * b.min);
      return Indicator(name + " " + b.name, unit * b.unit, newmin, newmax);
    }
  
    Indicator operator/(const Indicator& b) const {
      double newmin = std::min(min / b.min, std::min(min / b.max, b.min / max));
      double newmax = std::max(max / b.max, min / b.min);
      return Indicator("(" + name + ")/(" + b.name + ")", unit / b.unit, newmin, newmax);
    }

    // Serialization

    ostream& streamInsert(ostream& os) const {
      return unit.streamInsert(os) << name << endl << min << " " << max << " ";
    }

    static Indicator streamExtract(istream& in) {
      Unit unit = Unit::streamExtract(in);
      string name;
      getline(in, name);

      double min, max;
      in >> min >> max;

      return Indicator(name, unit, min, max);
    }
  };
}

#endif
