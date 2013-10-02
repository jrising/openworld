/******************************************************************************\
 * NamedVariable
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef NAMED_VARIABLE_H
#define NAMED_VARIABLE_H

#include "TemporalVariable.h"
#include "../memory/Transients.h"
#include "../dims/Dims.h"

namespace openworld {
  class NamedVariable : public TemporalVariable {
  protected:
    TemporalVariable* source;
	
  public:
  NamedVariable(string name, Unit unit, VariableHolder& holder)
    : TemporalVariable(name, unit, holder) {
      source = NULL;
    }

    virtual double evaluate(double time) {
      return source->evaluate(time);
    }

    virtual double getDouble() {
      return source->getDouble();
    }

    virtual TemporalVariable& operator=(TemporalVariable& value) {
      source = &value;
      Transients::abandon(source);
      return *this;
    }
  };
}

#endif
