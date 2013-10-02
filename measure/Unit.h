/******************************************************************************\
 * Unit: a class of measurements
 ******************************************************************************
 *
\******************************************************************************/

#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <iostream>
#include <stdexcept>
#include "../dims/Dimensions.h"
#include "../dims/Dims.h"

using namespace std;

namespace openworld {
  class Unit {
  protected:
    // typically only one of the following is used
    string name;
    map<Unit, double> factors;

    Dimensions dims;
		
  public:
    Unit(string name, Dimensions dims)
      : dims(dims) {
      this->name = name;
    }
    
    Unit(map<Unit, double> factors, Dimensions dims)
      : dims(dims) {
      this->name = "";
      this->factors = factors;
    }

    string toString() const {
      stringstream ss;
      ss << *this;
      return ss.str();
    }

    ostream& outNoDims(ostream& out) const {
      if (!this->name.empty())
        out << this->name;
      else {
        for (map<Unit, double>::const_iterator it = this->factors.begin(); it != this->factors.end(); it++) {
          if (it != this->factors.begin())
            out << " ";

          if (it->first.name.empty()) {
            out << "(";
            it->first.outNoDims(out);
            out << ")";
          } else
            it->first.outNoDims(out);
          if (it->second != 1)
            out << "^" << it->second;
        }
      }

      return out;
    }

    friend ostream& operator<<(ostream& out, const Unit& xx) {
      xx.outNoDims(out);
      return out << " [" << xx.dims << "]";
    }
  
    string getName() {
      return name;
    }
		
    Dimensions getDimensions() const {
      return dims;
    }

    // This function is needed to use as keys in a map
    // Modified from Dimensions.h
    bool operator<(const Unit& other) const {
      if (!name.empty() && !other.name.empty())
        return name < other.name;

      if (!name.empty())
        return true;

      if (!other.name.empty())
        return false;

      return factors < other.factors;
    }

    bool operator==(const Unit& b) const {
      if (this == &b)
        return true;

      if (!name.empty() || !b.name.empty())
        return name == b.name;

      if (dims != b.dims) {
        cout << dims << " != " << b.dims << endl;
        return false;
      }

      for (map<Unit, double>::const_iterator it = factors.begin(); it != factors.end(); it++)
        if (b.factors.count(it->first) == 0 || it->second != b.factors.find(it->first)->second) {
          cout << "A fails" << endl;
          return false;
        }
        
      for (map<Unit, double>::const_iterator it = b.factors.begin(); it != b.factors.end(); it++)
        if (factors.count(it->first) == 0 || it->second != factors.find(it->first)->second) {
          cout << "B fails" << endl;
          return false;
        }

      return true;

    }

    bool operator!=(const Unit& b) const {
      return !(*this == b);
    }

    // Mathematics
		
    friend Unit operator-(const Unit& a) {
      return Unit("-" + a.name, a.dims);
    }

    Unit operator+(const Unit& b) const {
      if (dims != b.dims)
        throw runtime_error("dimensions mismatch to +");
      return Unit("(" + name + " + " + b.name + ")", dims);
    }
		
    Unit operator-(const Unit& b) const {
      if (dims != b.dims) {
        cout << dims << " <> " << b.dims << endl;
        throw runtime_error("dimensions mismatch to -");
      }
      return Unit("(" + name + " - " + b.name + ")", dims);
    }

    Unit operator*(const Unit& b) const;
    Unit operator/(const Unit& b) const;

    friend Unit operator/(double a, const Unit& b) {
      if (a == 1)
        return b.raisedTo(-1);

      // XXX: replace this with logic that constracts set of factors, which includes a factor for 'a'
      ostringstream stringStream;
      stringStream << a << "/(" << b.name << ")";
      return Unit(stringStream.str(), Dims::none() / b.dims);
    }

    // Modified from Dimensions.h
    Unit raisedTo(double power) const {
      if (power == 1)
        return *this;

      map<Unit, double> newfactors;
      if (!name.empty())
        newfactors[*this] = power;
      else {
        for (map<Unit, double>::const_iterator it = factors.begin(); it != factors.end(); it++)
          newfactors[it->first] = it->second * power;

        if (newfactors.size() == 1 && newfactors.begin()->second == 1)
          return newfactors.begin()->first;
      }
	
      return Unit(newfactors, dims.raisedTo(power));
    }

    // Serialization

    ostream& streamInsert(ostream& os) const {
      return dims.streamInsert(os) << name << endl;
    }

    static Unit streamExtract(istream& in) {
      Dimensions dims = Dimensions::streamExtract(in);
      string name;
      getline(in, name);

      return Unit(name, dims);
    }
  };
}

#endif
