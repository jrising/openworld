#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include "../memory/Transients.h"

namespace openworld {
  class Distribution : public IDeletable {
  public:
    virtual double getMode() = 0;
    virtual double getMaxy() = 0;
    virtual Distribution& merge(Distribution& other, bool flippable = true) = 0;
    virtual void mergePurge() = 0;
    virtual double random(unsigned seed) = 0;

    friend Distribution& operator-(double a, Distribution& b);
    Distribution& operator+(Distribution& b);
    Distribution& operator*(Distribution& b);
    Distribution& operator/(Distribution& b);
  };

  class ContinuousDistribution : public Distribution {
  public:
    virtual double pdf(double x) = 0;
  };
}

#endif
