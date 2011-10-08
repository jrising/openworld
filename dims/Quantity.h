#ifndef QUANTITY_H
#define QUANTITY_H

#include "Dimensions.h"

namespace openworld {
  class Quantity {
  protected:
    double value;
    Dimensions dims;

  public:
    Quantity(double value, Dimensions dims) 
      : dims(dims) {
      this->value = value;
    }
    
    friend ostream& operator<<(ostream& out, const Quantity& xx) {
      return out << xx.value << " " << xx.dims;
    }
  
    double getValue() const {
      return value;
    }

    Dimensions getDimensions() const {
      return dims;
    }

    // setting-- must maintain dimensions
    Quantity& operator=(const Quantity& b) {
      if (dims != b.dims)
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
      if (dims != b.dims)
        throw runtime_error("Mismatched dimensions in Quantity < Quantity");

      return value < b.value;
    }

    friend Quantity operator-(Quantity& a) {
      return Quantity(-a.value, a.dims);
    }

    Quantity operator+(const Quantity& b) const {
      if (dims != b.dims)
        throw runtime_error("Mismatched dimensions in Quantity + Quantity");

      return Quantity(value + b.value, dims);
    }
		
    Quantity operator-(const Quantity& b) const {
      if (dims != b.dims)
        throw runtime_error("Mismatched dimensions in Quantity - Quantity");

      return Quantity(value - b.value, dims);
    }
		
    Quantity operator*(const Quantity& b) const {
      return Quantity(value * b.value, dims * b.dims);
    }
  
    Quantity operator/(const Quantity& b) const {
      return Quantity(value / b.value, dims / b.dims);
    }

    Quantity operator*(double b) const {
      return Quantity(value * b, dims);
    }
  
    Quantity operator/(double b) const {
      return Quantity(value / b, dims);
    }
  };
}

#endif
