#include "TemporalVariable.h"
#include "Function.h"
#include "Constant.h"
#include <measure/Units.h>

namespace openworld {
  stack<Variable*> TemporalVariable::evalstack;

  double func_negate(double args[]) {
    return -args[0];
  }

  //Variable operator-(const TemporalVariable& a) {
  TemporalVariable& TemporalVariable::operator-() {
    return *tew_(Function("-" + name, func_negate, unit, holder, *this));
  }

  double func_sum(double args[]) {
    return args[0] + args[1];
  }
			
  TemporalVariable& TemporalVariable::operator+(TemporalVariable& b) {
    if (unit != b.unit)
      throw runtime_error("Dimensions mismatch to +");
    return *tew_(Function("(" + name + " + " + b.name + ")", func_sum, unit, holder, *this, b));
  }
		
  TemporalVariable& TemporalVariable::operator+(double b) {
    if (unit != Units::none)
      throw runtime_error("Dimensions mismatch to + #");

    ostringstream stringStream;
    stringStream << "(" << name << " + " << b << ")";

    return *tew_(Function(stringStream.str(), func_sum, unit, holder, *this, *tew_(Constant("", b, Units::none, holder))));
  }
		
  double func_diff(double args[]) {
    return args[0] - args[1];
  }

  TemporalVariable& TemporalVariable::operator-(TemporalVariable& b) {
    if (unit != b.unit) {
      cout << unit << " <> " << b.unit << endl;
      throw runtime_error("Dimensions mismatch to -");
    }
    return *tew_(Function("(" + name + " - " + b.name + ")", func_diff, unit, holder, *this, b));
  }

  TemporalVariable& operator-(double a, TemporalVariable& b) {
    if (b.unit != Units::none)
      throw runtime_error("Dimensions mismatch to K -");
    
    ostringstream stringStream;
    stringStream << "(" << a << " - " << b.name << ")";
    
    return *tew_(Function(stringStream.str(), func_diff, b.unit, b.holder, *tew_(Constant("", a, Units::none, b.holder)), b));
  }
  	
  double func_mult(double args[]) {
    return args[0] * args[1];
  }
	
  TemporalVariable& TemporalVariable::operator*(TemporalVariable& b) {
    return *tew_(Function(name + " " + b.name, func_mult, unit * b.unit, holder, *this, b));
  }
    
  double func_div(double args[]) {
    return args[0] / args[1];
  }

  TemporalVariable& TemporalVariable::operator/(TemporalVariable& b) {
    return *tew_(Function("(" + name + ")/(" + b.name + ")", func_div, unit / b.unit, holder, *this, b));
  }

  double func_min(double args[]) {
    return min(args[0], args[1]);
  }

  TemporalVariable& min(TemporalVariable& a, TemporalVariable& b) {
    if (a.unit != b.unit)
      throw runtime_error("Dimensions mismatch in min()");
    
    ostringstream stringStream;
    stringStream << "min(" << a.name << ", " << b.name << ")";
    
    return *tew_(Function(stringStream.str(), func_min, a.unit, b.holder, a, b));
  }

  double func_sqrt(double args[]) {
    return sqrt(args[0]);
  }

  TemporalVariable& sqrt(TemporalVariable& a) {
    ostringstream stringStream;
    stringStream << "sqrt(" << a.name << ")";
    
    return *tew_(Function(stringStream.str(), func_sqrt, a.unit.raisedTo(.5), a.holder, a));
  }
}
