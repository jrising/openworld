/******************************************************************************\
 * Stock: A TemporalVariable that changes by flows (its derivative)
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef OW_STOCK_H
#define OW_STOCK_H

#include <list>
#include <algorithm>
#include "TemporalVariable.h"
#include <memory/Transients.h>
#include "../dims/Dims.h"

namespace openworld {
  class Stock : public TemporalVariable {
  protected:
    list<TemporalVariable*> args;
    double level;
	
  public:
    static double deltat;

  Stock(string name, double initial, Unit unit, VariableHolder& holder)
    : TemporalVariable(name, unit, holder) {
      level = initial;
    }

    virtual double getDouble() {
      return level;
    }

    virtual Variable& operator=(double value) {
      level = value;
      return *this;
    }

    friend ostream& operator<<(ostream& out, const Stock& xx) {
      return out << "Stock " << xx.toString();
    }
    
    double getLevel() {
      return level;
    }

    void add(double x) {
      level += x;
    }

    void setArguments(list<TemporalVariable*>& args) {
      this->args = args;
    }

    virtual TemporalVariable& dt();

    virtual double evaluate(double time) {
      if (time <= this->time && time >= this->time - deltat - deltat*deltat)
        return level;
      return TemporalVariable::evaluate(time);
    }

    virtual double evaluateInternal(double time) {
      for (double tt = this->time; tt <= time - deltat; tt += min(deltat, time - deltat)) {
        holder.evaluate(tt);
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
  StockFlow(Stock stock, Unit unit, VariableHolder& holder)
    : TemporalVariable(stock.getName() + ".dt", unit, holder), stock(stock) {
    }

    friend ostream& operator<<(ostream& out, const StockFlow& xx) {
      return out << "Stock.dt " << xx.toString();
    }

    virtual TemporalVariable& operator=(TemporalVariable& value) {
      list<TemporalVariable*> args;
      args.push_back(&value);

      stock.setArguments(args);
      return *this;
    }

    virtual ~StockFlow() { }
  };
}

#endif
