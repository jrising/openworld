#ifndef DIMENSIONS_H
#define DIMENSIONS_H

#include <string>
#include <iostream>
#include <map>
#include <sstream>

using namespace std;

namespace openworld {
  class Dimensions {
  protected:
    // An instance represents a particular substance
    // will have one or the other of the following
    string name;
    map<Dimensions, double> factors;

  public:
    Dimensions(string name)  {
      this->name = name;
    }

    Dimensions(map<Dimensions, double> factors) {
      this->name = "";
      this->factors = factors;
    }
		
    Dimensions(string name, map<Dimensions, double> factors) {
      this->name = name;
      this->factors = factors;
    }

    Dimensions(const Dimensions& other) {
      this->name = other.name;
      this->factors = other.factors;      
    }
	
    string toString() {
      stringstream ss;
      ss << *this;
      return ss.str();
    }

    friend ostream& operator<<(ostream& out, const Dimensions& xx) {
      if (!xx.name.empty())
        out << xx.name;
      else {
        for (map<Dimensions, double>::const_iterator it = xx.factors.begin(); it != xx.factors.end(); it++) {
          if (it != xx.factors.begin())
            cout << " ";
          if (it->first.name.empty())
            out << "(" << it->first << ")";
          else
            out << it->first;
          if (it->second != 1)
            out << "^" << it->second;
        }
      }

      return out;
    }

    map<Dimensions, double> getFactors() {
      return factors;
    }
	
    // This function is needed to use as keys in a map
    bool operator<(const Dimensions& other) const {
      if (!name.empty() && !other.name.empty())
        return name < other.name;

      if (!name.empty())
        return true;

      if (!other.name.empty())
        return false;

      return factors < other.factors;
    }

    bool operator==(const Dimensions& other) const {
      if (!name.empty() || !other.name.empty())
        return name == other.name;

      for (map<Dimensions, double>::const_iterator it = factors.begin(); it != factors.end(); it++)
        if (other.factors.count(it->first) == 0 || it->second != other.factors.find(it->first)->second)
          return false;
        
      for (map<Dimensions, double>::const_iterator it = other.factors.begin(); it != other.factors.end(); it++)
        if (factors.count(it->first) == 0 || it->second != factors.find(it->first)->second)
          return false;

      return true;
    }

    bool operator!=(const Dimensions& other) const {
      return !(*this == other);
    }

    Dimensions operator*(const Dimensions& other) const {
      map<Dimensions, double> newfactors;
    
      if (!name.empty())
        newfactors[*this] = 1.0;
      else {
        for (map<Dimensions, double>::const_iterator it = factors.begin(); it != factors.end(); it++)
          newfactors[it->first] = it->second;
      }
	
      if (!other.name.empty()) {
        if (newfactors.count(other) > 0)
          newfactors[other]++;
        else
          newfactors[other] = 1.0;
      } else {
        for (map<Dimensions, double>::const_iterator it = other.factors.begin(); it != other.factors.end(); it++)
          if (newfactors.count(it->first) > 0)
            newfactors[it->first] += it->second;
          else
            newfactors[it->first] = it->second;
      }
      
      // Remove all elements with power 0
      map<Dimensions, double> newfactors2;
      for (map<Dimensions, double>::const_iterator it = newfactors.begin(); it != newfactors.end(); it++)
        if (it->second != 0)
          newfactors2[it->first] = it->second;

      return Dimensions(newfactors2);
    }

    Dimensions raisedTo(double power) const {
      map<Dimensions, double> newfactors;
      if (!name.empty())
        newfactors[*this] = power;
      else {
        for (map<Dimensions, double>::const_iterator it = factors.begin(); it != factors.end(); it++)
          newfactors[it->first] = it->second * power;
      }
	
      return Dimensions(newfactors);
    }
		
    Dimensions operator/(const Dimensions& other) const {
      return *this * (other.raisedTo(-1));
    }

    // Serialization

    virtual ostream& streamInsert(ostream& os) const {
      os << name << endl;

      os << factors.size() << " ";
      for (map<Dimensions, double>::const_iterator it = factors.begin() ; it != factors.end(); it++)
        it->first.streamInsert(os) << it->second << " ";

      return os;
    }

    static Dimensions streamExtract(istream& in);
  };
}

#endif
