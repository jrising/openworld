/******************************************************************************\
 * TemporalVariable: A variable that changes in time
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef TEMPORAL_VARIABLE_H
#define TEMPORAL_VARIABLE_H

#include <stack>
#include "Variable.h"

namespace openworld {
  class TemporalVariable : public Variable {
  protected:
    bool evaluating;
    double time;

    //void (*UpdateHandler)(object sender, double time, double value);
    //event UpdateHandler Updated;

    static stack<Variable*> evalstack;

    virtual double evaluateInternal(double time) {
      return 0;
    }
	
  public:
  TemporalVariable(string name, Unit unit)
    : Variable(name, unit) {
      this->evaluating = false;
      this->time = 0;
    }
		
    double getCurrentTime() {
      return time;
    }
    
    /*virtual TemporalVariable& dt() {
      return NULL; // not defined yet
      }*/
		
    virtual double evaluate(double time) {
      if (time < this->time)
        throw runtime_error("Time before now");
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
	
    friend Variable operator-(const TemporalVariable& a);		
    TemporalVariable operator+(const TemporalVariable& b);		
    TemporalVariable operator+(double b);		
    TemporalVariable operator-(const TemporalVariable& b);
    TemporalVariable operator*(const TemporalVariable& b);    
    TemporalVariable operator/(const TemporalVariable& b);
  };
}

#endif
