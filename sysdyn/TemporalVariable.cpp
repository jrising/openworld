#include "TemporalVariable.h"
#include "Function.h"

Variable operator-(const TemporalVariable& a) {
  return Function("-" + a.name, func_negate, a.dims, a);
}
	
double func_negate(double args[]) {
  return -args[0];
}
		
TemporalVariable TemporalVariable::operator+(const TemporalVariable& b) {
  if (dims != b.dims)
    throw runtime_error("Dimensions mismatch to +");
  return Function("(" + name + " + " + b.name + ")", func_sum, dims, *this, b);
}

double func_sum(double args[]) {
  return args[0] + args[1];
}
		
TemporalVariable TemporalVariable::operator+(double b) {
  if (Dimensionless.is(dims))
    throw runtime_error("Dimensions mismatch to + #");
  return Function("(" + name + " + " + b + ")", func_sum, dims, a, b);
}
		
TemporalVariable TemporalVariable::operator-(const TemporalVariable& b) {
  if (dims != b.dims)
    throw runtime_error("Dimensions mismatch to -");
  return Function("(" + name + " - " + b.name + ")", func_diff, dims, a, b);
}

double func_diff(double args[]) {
  return args[0] - args[1];
}
		
TemporalVariable TemporalVariable::operator*(const TemporalVariable& b) {
  return Function(name + " " + b.name, func_mult, dims * b.dims, a, b);
}
    
double func_mult(double args[]) {
  return args[0] * args[1];
}

TemporalVariable TemporalVariable::operator/(const TemporalVariable& b) {
  return Function("(" + name + ")/(" + b.name + ")", func_div, dims / b.dims, a, b);
}

double func_div(double args[]) {
  return args[0] / args[1];
}
