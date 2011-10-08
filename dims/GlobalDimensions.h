#ifndef GLOBAL_DIMENSIONS_H
#define GLOBAL_DIMENSIONS_H

#include <stdexcept>
#include "Dimensions.h"

using namespace std;

namespace openworld {
  class GlobalDimensions : public Dimensions {
  protected:
    static map<string, Dimensions*> dict;
    static bool initialized;

  public:
    GlobalDimensions(string name)
      : Dimensions(name) {
    }
				
    GlobalDimensions(map<Dimensions, double> factors)
      : Dimensions(factors) {
    }
		
    GlobalDimensions(string name, map<Dimensions, double> factors)
      : Dimensions(name, factors) {
    }
  
    static Dimensions& get(string name) {
      if (!initialized) {
        dict = map<string, Dimensions*>();
        initialized = true;
      }

      if (dict.count(name) > 0)
        return *(dict.find(name)->second);
    
      Dimensions* dims = new GlobalDimensions(name);
      dict.insert(make_pair(name, dims));
      return *dims;
    }
		
    static Dimensions& add(string name, Dimensions dims) {
      if (dict.count(name) > 0)
        throw runtime_error("GlobalDimension already defined");
      
      Dimensions* newdims = new GlobalDimensions(name, dims.getFactors());
      dict.insert(make_pair(name, newdims));
      return *newdims;
    }
  };
}

#endif
