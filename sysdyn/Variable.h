/******************************************************************************\
 * Variable: Any kind of value which contructs before it runs
 ******************************************************************************
 *
\******************************************************************************/
#ifndef VARIABLE_H
#define VARIABLE_H

#include <measure/Unit.h>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace openworld {
  class Variable {
  protected:
    string name;
    Unit unit;

  public:
    Variable(string name, Unit unit)
      : name(name), unit(unit) {
    }

    string toString() const {
      return name + "[" + unit.toString() + "]";
    }

    string getName() {
      return name;
    }

    Unit getUnit() {
      return unit;
    }

    virtual double getDouble() {
      return 0;
    }

    virtual Variable& operator=(double value) {
      return *this;
    }

    // Mathematics

    friend Variable operator-(const Variable& a) {
      return Variable("-" + a.name, a.unit);
    }

    Variable operator+(const Variable b) {
      if (unit != b.unit)
        throw runtime_error("Unit mismatch to +");
      return Variable("(" + name + " + " + b.name + ")", unit);
    }

    Variable operator-(Variable b) {
      if (unit != b.unit)
        throw runtime_error("Unit mismatch to -");
      return Variable("(" + name + " - " + b.name + ")", unit);
    }

    Variable operator*(Variable b) {
      return Variable(name + " " + b.name, unit * b.unit);
    }

    Variable operator/(Variable b) {
      return Variable("(" + name + ")/(" + b.name + ")", unit / b.unit);
    }
  };
}

#endif
