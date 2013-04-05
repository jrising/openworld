/******************************************************************************\
 * TemporalVariable: A variable that changes in time
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef TEMPORAL_VARIABLE_H
#define TEMPORAL_VARIABLE_H

namespace openworld {
  class TemporalVariable : public Variable {
  protected:
    bool evaluating = false;
    double time = 0;

    void (*UpdateHandler)(object sender, double time, double value);
    event UpdateHandler Updated;

    static Stack<Variable> evalstack = new Stack<Variable>();

    double EvaluateInternal(double time) = NULL;
	
  public:
  TemporalVariable(string name, Dimensions dims)
    : base(name, dims) {
    }
		
    double getCurrentTime() {
      return time;
    }
		
    virtual double Evaluate(double time) {
      if (time < this.time)
        throw new ArgumentOutOfRangeException("Time before now");
      if (evaluating) {
        // Construct trace
        StringBuilder trace = new StringBuilder();
        trace.AppendLine(Name);
        foreach (Variable v in evalstack.ToArray())
          trace.AppendLine(v.Name);
        throw new ExecutionEngineException("Reciprical definitions: " + trace.ToString());
      }
      
      this.evaluating = true;
      evalstack.Push(this);
      double result = EvaluateInternal(time);
      this.time = time;
      if (evalstack.Pop() != this)
        throw new ExecutionEngineException("Out of order iterations");
      this.evaluating = false;
      
      if (Updated != null)
        Updated(this, time, result);
			
      return result;
    }
		
    static TemporalVariable operator-(TemporalVariable a) {
      return new Function("-" + a.Name, x => -x[0], a.Dimensions, a);
    }
		
    TemporalVariable operator+(TemporalVariable a, TemporalVariable b) {
      if (dims != b.dims)
        throw new ArgumentException("Dimensions mismatch to +");
      return Function("(" + name + " + " + b.name + ")", x => x[0] + x[1], dims, a, b);
    }
		
    TemporalVariable operator+(TemporalVariable a, double b) {
      if (dims != Dimensionless.Instance)
        throw new ArgumentException("Dimensions mismatch to + #");
      return Function("(" + name + " + " + b + ")", x => x[0] + b, dims, a);
    }
		
    TemporalVariable operator-(TemporalVariable a, TemporalVariable b) {
      if (dims != b.dims)
        throw new ArgumentException("Dimensions mismatch to -");
      return Function("(" + name + " - " + b.name + ")", x => x[0] - x[1], dims, a, b);
    }
		
    TemporalVariable operator*(TemporalVariable a, TemporalVariable b) {
      return Function(name + " " + b.name, x => x[0] * x[1], dims * b.dims, a, b);
    }
    
    TemporalVariable operator/(TemporalVariable a, TemporalVariable b) {
      return Function("(" + name + ")/(" + b.name + ")", x => x[0] / x[1], dims / b.dims, a, b);
    }
  };
}

#endif
