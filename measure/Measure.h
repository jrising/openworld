/******************************************************************************\
 * Measure: Measures have dimensions and a frame of reference
 ******************************************************************************
\******************************************************************************/
#ifndef MEASURE_H
#define MEASURE_H

#include "Indicator.h"
#include "Quantity.h"
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

    Measure random() {
      return Measure(indicator.getRandomValue(), indicator);
    }

    // scale > 0, scale of 1 means up-to about-double
    Measure randomAdjust(double scale) {
      double diff = getValue() - indicator.getMin();
      double f = (double) rand() / RAND_MAX;
      double value = diff * (1 + 2 * (f - .5) * scale) + indicator.getMin();
      if (diff == 0) // special case: allow jumps anywhere
        value = f * (indicator.getMax() - indicator.getMin()) * scale + indicator.getMin();
      
      if (value > indicator.getMax())
        value = indicator.getMax();
      return Measure(value, indicator);
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

    bool operator!=(const Measure& b) const {
      return !(value == b.value);
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

      return Quantity(value - b.value, indicator.getUnit());
    }

    Measure operator+(const Quantity& b) const {
      if (indicator.getUnit() != b.getUnit())
        throw runtime_error("Mismatched dimensions");

      return Measure(value + b.getValue(), indicator);
    }

    Measure& operator+=(const Quantity& b) {
      if (indicator.getUnit() != b.getUnit())
        throw runtime_error("Mismatched dimensions");

      value += b.getValue();
      return *this;
    }
		
    Measure operator-(const Quantity& b) const {
      if (indicator.getUnit() != b.getUnit())
        throw runtime_error("Mismatched dimensions");

      return Measure(value - b.getValue(), indicator);
    }

    ostream& streamInsert(ostream& os) const {
      return indicator.streamInsert(os) << value << " ";
    }

    static Measure streamExtract(istream& in) {
      Indicator indicator = Indicator::streamExtract(in);
      double value;
      in >> value;

      return Measure(value, indicator);
    }
  };
}

#endif
