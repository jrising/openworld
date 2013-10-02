/******************************************************************************\
 * TemporalModel
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef TEMPORAL_MODEL_H
#define TEMPORAL_MODEL_H

#include "TemporalVariable.h"

namespace openworld {
  class TemporalModel : public VariableHolder {
  protected:
    list<TemporalVariable*> variables;
    double time;

  public:
    virtual void addVariable(Variable* variable) {
      TemporalVariable* tempvar = dynamic_cast<TemporalVariable*>(variable);
      if (tempvar)
        variables.push_back(tempvar);
    }

    void evaluate(double time) {
      if (this->time == time)
        return;

      for (list<TemporalVariable*>::iterator it = variables.begin(); it != variables.end(); it++)
        (*it)->evaluate(time);
      this->time = time;
    }
  };
}

#endif
