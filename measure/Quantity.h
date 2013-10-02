/******************************************************************************\
 * Quantity: Quantities have dimensions, but no frame of reference
 ******************************************************************************
\******************************************************************************/
#ifndef QUANTITY_H
#define QUANTITY_H

#include "Unit.h"

namespace openworld {
  class Quantity {
  protected:
    double value;
    Unit unit;

  public:
    Quantity(double value, Unit unit) 
      : unit(unit) {
      this->value = value;
    }
    
    friend ostream& operator<<(ostream& out, const Quantity& xx) {
      return out << xx.value << " " << xx.unit;
    }
  
    double getValue() const {
      return value;
    }

    Unit getUnit() const {
      return unit;
    }

    // setting-- must maintain dimensions
    Quantity& operator=(const Quantity& b) {
      if (unit != b.unit)
        throw runtime_error("Mismatched dimensions");

      value = b.value;

      return *this;
    }

    Quantity& operator=(double v) {
      value = v;

      return *this;
    }

    Quantity& operator+=(double v) {
      value += v;
      return *this;
    }

    // Mathematics
		
    bool operator<(const Quantity& b) const {
      if (unit != b.unit)
        throw runtime_error("Mismatched dimensions in Quantity < Quantity");

      return value < b.value;
    }

    friend Quantity operator-(Quantity& a) {
      return Quantity(-a.value, a.unit);
    }

    Quantity operator+(const Quantity& b) const {
      if (unit != b.unit)
        throw runtime_error("Mismatched dimensions in Quantity + Quantity");

      return Quantity(value + b.value, unit);
    }
		
    Quantity operator-(const Quantity& b) const {
      if (unit != b.unit)
        throw runtime_error("Mismatched dimensions in Quantity - Quantity");

      return Quantity(value - b.value, unit);
    }
		
    Quantity operator*(const Quantity& b) const {
      return Quantity(value * b.value, unit * b.unit);
    }
  
    Quantity operator/(const Quantity& b) const {
      return Quantity(value / b.value, unit / b.unit);
    }

    Quantity operator*(double b) const {
      return Quantity(value * b, unit);
    }
  
    Quantity operator/(double b) const {
      return Quantity(value / b, unit);
    }
  };
}

#endif
