/******************************************************************************\
 * MultiMeasure: More than one value within a single measure (list or array)
 ******************************************************************************
\******************************************************************************/
#ifndef MULTI_MEASURE_H
#define MULTI_MEASURE_H

#include "Indicator.h"
#include "../dims/Quantity.h"
#include <utility>

namespace openworld {
  class MultiMeasure {
  protected:
    unsigned length;
    double* values;
    Indicator indicator;

  public:
    MultiMeasure(unsigned length, double* values, Indicator indicator) 
      : indicator(indicator) {
      this->length = length;
      this->values = values;
    }
    
    double getValue(unsigned ii) {
      return values[ii];
    }

    Indicator getIndicator() {
      return indicator;
    }

    // setting-- must maintain indicator
    MultiMeasure& operator=(const MultiMeasure& b) {
      if (indicator != b.indicator)
        throw runtime_error("Mismatched indicators");
      if (length != b.length)
        throw runtime_error("Mismatched sizes");

      for (unsigned ii = 0; ii < length; ii++)
        values[ii] = b.values[ii];

      return *this;
    }

    // Mathematics
		
    bool operator==(const MultiMeasure& b) const {
      if (indicator != b.indicator)
        throw runtime_error("Mismatched indicators in MultiMeasure == MultiMeasure");
      if (length != b.length)
        throw runtime_error("Mismatched sizes in MultiMeasure == MultiMeasure");

      for (unsigned ii = 0; ii < length; ii++)
        if (values[ii] != b.values[ii])
          return false;

      return true;
    }

    bool operator!=(const MultiMeasure& b) const {
      return !(*this == b);
    }
  };
}

#endif
