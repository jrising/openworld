/******************************************************************************\
 * Constant: A TemporalVariable that is constant
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef CONSTANT_H
#define CONSTANT_H

#include "TemporalVariable.h"
#include "../dims/Dims.h"

namespace openworld {
  class Constant : public TemporalVariable {
  protected:
    double constant;
	
    virtual double evaluateInternal(double time)
    {
      return constant;
    }

  public:
  Constant(string name, double constant, Unit unit, VariableHolder& holder)
    : TemporalVariable(name, unit, holder) {
      this->constant = constant;
    }

    virtual double evaluate(double time) {
      return constant;
    }

    virtual double getDouble() {
      return constant;
    }

    virtual Variable& operator=(double value) {
      constant = value;
      return *this;
    }
  };
}

#endif
