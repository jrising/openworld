/******************************************************************************\
 * Memoized variable: a variable that can store is value for a point in time
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef MEMOIZED_VARIABLE_H
#define MEMOIZED_VARIABLE_H

namespace openworld {
  class MemoizedVariable : public TemporalVariable {
  protected:
    double value;
    bool evaled = false;
    
    double EvaluateUpdate(double time) = NULL;

    virtual double EvaluateInternal(double time) {
      if (!evaled || this.time != time) {
        value = EvaluateUpdate(time);
        evaled = true;
      }
			
      return value;
    }
  
  public:
  MemoizedVariable(string name, Dimensions dims)
    : base(name, dims) {
      evaled = false;
    }
		
    virtual double Evaluate(double time) {
      if (!evaled && this.time == time)
        return value;
      return base.Evaluate(time);
    }		
		
    double getValue() {
      return value;
    }
  };
}

#endif
