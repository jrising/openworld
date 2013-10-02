#ifndef DELTA_DISTRIBUTION_H
#define DELTA_DISTRIBUTION_H

namespace openworld {
  class DeltaDistribution : public ContinuousDistribution {
  protected:
    double value;

  public:
    DeltaDistribution(double value) {
      this->value = value;
    }

    virtual double getMode() {
      return value;
    }

    virtual double getMaxy() {
      return value;
    }

    virtual Distribution& merge(Distribution& other, bool flippable = true) {
      if (dynamic_cast<DeltaDistribution*>(&other) != NULL)
        if (other.getMode() != value)
          throw runtime_error("Cannot merge two delta distributions.");
      
      return *this;
    }

    virtual void mergePurge() {}

    virtual double random(unsigned seed) {
      return value;
    }

    virtual double pdf(double x) {
      if (x == value)
        return INFINITY;
      else
        return 0;
    }
  };
}

#endif
