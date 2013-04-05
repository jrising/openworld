/******************************************************************************\
 * Dimensions: 
 ******************************************************************************
 *
\******************************************************************************/

#include "Dimensions.h"
#include "Dimensionless.h"

using namespace openworld;

Dimensions Dimensions::streamExtract(istream& in) {
  string name;
  unsigned count;
  map<Dimensions, double> factors;
  
  getline(in, name);
  if (!name.compare(STREAM_CODE_DIMLESS))
    return Dimensionless::getInstance();
  
  in >> count;
  
  for (unsigned ii = 0; ii < count; ii++) {
    Dimensions dims = streamExtract(in);
    double factor;
    in >> factor;
    
    factors.insert(pair<Dimensions, double>(dims, factor));
  }

  return Dimensions(name, factors);
}
