/******************************************************************************\
 * Function: A memoized variable that evaluates a function
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef FUNCTION_H
#define FUNCTION_H

namespace openworld {
  class Function : public MemoizedVariable {
  protected:
    //public delegate double Evaluator(double[] args);
		
    double (*function)(double[] args);
    TemporalVariable[] args;

    virtual double EvaluateUpdate(double time) {
      double[] vals = new double[args.Length];
      for (int ii = 0; ii < args.Length; ii++) {
        vals[ii] = args[ii].Evaluate(time);
      }
      
      return function(vals);
    }

  public:
  Function(string name, double (*function)(double[] args), Dimensions dims, TemporalVariable[] args)
    : base(name, dims) {
      this.function = function;
      this.args = args;
    }
  };
}

#endif
