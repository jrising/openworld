/******************************************************************************\
 * Function: A memoized variable that evaluates a function
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include "MemoizedVariable.h"
#include "TemporalVariable.h"

namespace openworld {
  class Function : public MemoizedVariable {
  protected:
    //public delegate double Evaluator(double[] args);
		
    double (*function)(double[]);
    vector<TemporalVariable*> args;

    virtual double evaluateUpdate(double time) {
      double* vals = new double[args.size()];
      for (unsigned ii = 0; ii < args.size(); ii++)
        vals[ii] = args[ii]->evaluate(time);
      
      double result = function(vals);
      delete[] vals;
      return result;
    }

  public:
  Function(string name, double (*function)(double[]), Unit unit, VariableHolder& holder, TemporalVariable& arg0)
    : MemoizedVariable(name, unit, holder) {
      this->function = function;
      args.push_back(&arg0);
    }

  Function(string name, double (*function)(double[]), Unit unit, VariableHolder& holder, TemporalVariable& arg0, TemporalVariable& arg1)
    : MemoizedVariable(name, unit, holder) {
      this->function = function;
      args.push_back(&arg0);
      args.push_back(&arg1);
    }
  };
}

#endif
