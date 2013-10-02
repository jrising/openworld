#ifndef SIMPLE_MODEL_H
#define SIMPLE_MODEL_H

#include <sysdyn/TemporalModel.h>

namespace openworld {
  class SimpleModel {
  public:
    virtual list<Variable> getVariables() = 0;
    virtual void setVariables(map<string, double>& vars) = 0;
    virtual map<string, double> evaluateVariables(double time) = 0;    
  };

  class SimpleTemporalModel : public SimpleModel, public TemporalModel {
  public:
    virtual list<Variable> getVariables() {
      list<Variable> vars;
      for (list<TemporalVariable*>::iterator it = variables.begin(); it != variables.end(); it++)
        vars.push_back(**it);

      return vars;
    }

    virtual void setVariables(map<string, double>& vars) {
      for (list<TemporalVariable*>::iterator it = variables.begin(); it != variables.end(); it++)
        if (vars.find((*it)->getName()) != vars.end()) {
          TemporalVariable& tempvar(**it);
          tempvar = vars[(*it)->getName()];
        }
    }

    virtual map<string, double> evaluateVariables(double time) {
      evaluate(time);

      map<string, double> vars;
      for (list<TemporalVariable*>::iterator it = variables.begin(); it != variables.end(); it++)
        vars[(*it)->getName()] = (*it)->getDouble();

      return vars;
    }
  };
}

#endif
