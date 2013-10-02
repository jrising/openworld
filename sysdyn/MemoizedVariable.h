/******************************************************************************\
 * Memoized variable: a variable that can store is value for a point in time
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef MEMOIZED_VARIABLE_H
#define MEMOIZED_VARIABLE_H

#include "TemporalVariable.h"

namespace openworld {
  class MemoizedVariable : public TemporalVariable {
  protected:
    double value;
    bool evaled;
    
    virtual double evaluateUpdate(double time) = 0;

    virtual double evaluateInternal(double time) {
      if (!evaled || this->time != time) {
        value = evaluateUpdate(time);
        evaled = true;
      }
			
      return value;
    }
  
  public:
  MemoizedVariable(string name, Unit unit, VariableHolder& holder)
    : TemporalVariable(name, unit, holder) {
      evaled = false;
      value = 0;
    }

    virtual double getDouble() {
      return value;
    }

    virtual Variable& operator=(double value) {
      this->value = value;
      return *this;
    }
		
    virtual double evaluate(double time) {
      if (evaled && this->time == time)
        return value;
      return TemporalVariable::evaluate(time);
    }
		
    double getValue() {
      return value;
    }
  };
}

#endif
