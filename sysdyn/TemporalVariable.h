/******************************************************************************\
 * TemporalVariable: A variable that changes in time
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef TEMPORAL_VARIABLE_H
#define TEMPORAL_VARIABLE_H

#include <stack>
#include <memory/Transients.h>
#include "Variable.h"

namespace openworld {
  class VariableHolder {
  public:
    virtual void addVariable(Variable* variable) = 0;
    virtual void evaluate(double time) = 0;
  };

  class DummyVariableHolder : public VariableHolder {
  public:
    virtual void addVariable(Variable* variable) { }
    virtual void evaluate(double time) { }
  };

  class TemporalVariable : public Variable, public IDeletable {
  protected:
    VariableHolder& holder;
    bool evaluating;
    double time;

    //void (*UpdateHandler)(object sender, double time, double value);
    //event UpdateHandler Updated;

    static stack<Variable*> evalstack;

    virtual double evaluateInternal(double time) {
      return 0;
    }
	
  public:
  TemporalVariable(string name, Unit unit, VariableHolder& holder, double time = 0)
    : Variable(name, unit), holder(holder) {
      this->evaluating = false;
      this->time = time;
      holder.addVariable(this);
    }
	
    virtual ~TemporalVariable() { }

    friend ostream& operator<<(ostream& out, const TemporalVariable& xx) {
      return out << xx.toString();
    }
	
    double getCurrentTime() {
      return time;
    }
    
    /*virtual TemporalVariable& dt() {
      return NULL; // not defined yet
      }*/

    virtual double evaluate(double time) {
      //cout << this->name << ": " << time << " >? " << this->time << endl;
      if (time < this->time) {
        throw runtime_error("Time before now");
      }
      if (evaluating) {
        // Construct trace
        string trace = name + "\n";
        while (!evalstack.empty()) {
          trace += evalstack.top()->getName() + "\n";
          evalstack.pop();
        }
        throw runtime_error("Reciprical definitions: " + trace);
      }
      
      this->evaluating = true;
      evalstack.push(this);
      double result = evaluateInternal(time);
      this->time = time;

      if (evalstack.top() != this)
        throw runtime_error("Out of order iterations");
      evalstack.pop();
      this->evaluating = false;
      
      //if (Updated != null)
      //  Updated(this, time, result);
			
      return result;
    }

    // operations
	
    //friend TemporalVariable operator-(const TemporalVariable& a);
    TemporalVariable& operator-();
    TemporalVariable& operator+(TemporalVariable& b);		
    TemporalVariable& operator+(double b);
    TemporalVariable& operator-(TemporalVariable& b);
    friend TemporalVariable& operator-(double a, TemporalVariable& b);
    TemporalVariable& operator*(TemporalVariable& b);    
    TemporalVariable& operator/(TemporalVariable& b);
    friend TemporalVariable& min(TemporalVariable& a, TemporalVariable& b);
    friend TemporalVariable& sqrt(TemporalVariable& a);

    virtual Variable& operator=(double value) {
      // ignore
      return *this;
    }

    virtual TemporalVariable& operator=(TemporalVariable& value) {
      throw runtime_error("= on a non-mutable temporal variable");
    }
  };
}

#endif
