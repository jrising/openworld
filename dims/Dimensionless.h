#ifndef DIMENSIONLESS_H
#define DIMENSIONLESS_H

#include "Dimensions.h"

#define STREAM_CODE_DIMLESS "NONE"

namespace openworld {
  class Dimensionless : public Dimensions {
  private:
    static Dimensionless* instance;
		
    Dimensionless()
      : Dimensions("", map<Dimensions, double>()) {
    }

  public:
    static Dimensionless& getInstance() {
      if (instance == NULL)
        instance = new Dimensionless();
      return *instance;
    }

    static bool is(Dimensions& dims) {
      return (&dims == &(getInstance()));
    }
		
    Dimensions raisedTo(double power) const {
      return *this;
    }

    Dimensions operator*(const Dimensions& other) const {
      return other;
    }
		
    Dimensions operator/(const Dimensions& other) const {
      return other.raisedTo(-1);
    }

    virtual ostream& streamInsert(ostream& os) const {
      os << STREAM_CODE_DIMLESS << endl;
      return os;
    }
  };
}

#endif
