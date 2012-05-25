#ifndef MEASURE_H
#define MEASURE_H

#include "Indicator.h"
#include "../dims/Quantity.h"
#include <utility>

namespace openworld {
  class Measure {
  protected:
    double value;
    Indicator indicator;

  public:
    Measure(double value, Indicator indicator) 
      : indicator(indicator) {
      this->value = value;
    }

    Measure(Indicator indicator) 
      : indicator(indicator) {
      this->value = 0;
    }
    
    friend ostream& operator<<(ostream& out, const Measure& xx) {
      return out << xx.value << " " << xx.indicator;
    }
  
    double getValue() {
      return value;
    }

    Indicator getIndicator() {
      return indicator;
    }

    // setting-- must maintain indicator
    Measure& operator=(const Measure& b) {
      if (indicator != b.indicator)
        throw runtime_error("Mismatched indicators");

      value = b.value;

      return *this;
    }

    Measure& operator=(double v) {
      value = v;

      return *this;
    }

    Measure& operator+=(double v) {
      value += v;
      return *this;
    }

    // Mathematics
		
    bool operator==(const Measure& b) const {
      if (indicator != b.indicator)
        throw runtime_error("Mismatched indicators in Measure == Measure");

      return value == b.value;
    }

    bool operator<(const Measure& b) const {
      if (indicator != b.indicator)
        throw runtime_error("Mismatched indicators in Measure < Measure");

      return value < b.value;
    }

    bool operator>=(const Measure& b) const {
      if (indicator != b.indicator)
        throw runtime_error("Mismatched indicators in Measure >= Measure");

      return value >= b.value;
    }
		
    Quantity operator-(const Measure& b) const {
      if (indicator != b.indicator)
        throw runtime_error("Mismatched indicators in Measure - Measure");

      return Quantity(value - b.value, indicator.getDimensions());
    }

    Measure operator+(const Quantity& b) const {
      if (indicator.getDimensions() != b.getDimensions())
        throw runtime_error("Mismatched dimensions");

      return Measure(value + b.getValue(), indicator);
    }

    Measure& operator+=(const Quantity& b) {
      if (indicator.getDimensions() != b.getDimensions())
        throw runtime_error("Mismatched dimensions");

      value += b.getValue();
      return *this;
    }
		
    Measure operator-(const Quantity& b) const {
      if (indicator.getDimensions() != b.getDimensions())
        throw runtime_error("Mismatched dimensions");

      return Measure(value - b.getValue(), indicator);
    }
  };
}

#endif
