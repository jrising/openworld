/******************************************************************************\
 * Stock: A TemporalVariable that changes by flows (its derivative)
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef STOCK_H
#define STOCK_H

#include <list>
#include <algorithm>
#include "TemporalVariable.h"
#include "../dims/Dims.h"

namespace openworld {
  class Stock : public TemporalVariable {
  protected:
    static const double deltat = .1;

    list<TemporalVariable*> args;
    double level;
	
  public:
    Stock(string name, double initial, Unit unit)
      : TemporalVariable(name, unit) {
      level = initial;
    }
    
    double getLevel() {
      return level;
    }

    void add(double x) {
      level += x;
    }

    void setArguments(list<TemporalVariable*> args) {
      this->args = args;
    }

    virtual TemporalVariable& dt();

    virtual double evaluate(double time) {
      if (this->time == time)
        return level;
      return TemporalVariable::evaluate(time);
    }

    virtual double evaluateInternal(double time) {
      for (double tt = this->time; tt <= time - deltat; tt += min(deltat, time - deltat)) {
        double delta = 0;

        for (list<TemporalVariable*>::iterator it = args.begin(); it != args.end(); it++)
          delta += (*it)->evaluate(tt);

        if (tt == time - deltat) {
          add(deltat * delta);
          break;
        } else
          add(min(deltat, time - deltat - tt) * delta);
        this->time = min(tt + deltat, time);
      }
			
      return level;
    }

    // Define connections
    void increasesBy(TemporalVariable& x) {
      if (unit.getDimensions() / Dims::time() != x.getUnit().getDimensions())
        throw runtime_error("Dimension mismatch");
      args.push_back(&x);
    }
  };

  class StockFlow : public TemporalVariable {
  protected:
    Stock& stock;
	
  public:
    StockFlow(Stock stock, Unit unit)
      : TemporalVariable(stock.getName() + ".dt", unit), stock(stock) {
    }

    StockFlow& operator=(TemporalVariable& value) {
      list<TemporalVariable*> args;
      args.push_back(&value);

      stock.setArguments(args);
      return *this;
    }
  };
}

#endif
