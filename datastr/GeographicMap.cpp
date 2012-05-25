/******************************************************************************\
 * GeographicMap: A collection of data about the surface of the planet
 ******************************************************************************/

#include "GeographicMap.h"

namespace openworld {
  GeographicMap<double>& operator*(double one, const GeographicMap<bool>& two) {
    GeographicMap<double>* result = tew_(MatrixGeographicMap<double>(two.getLatitudes(), two.getLongitudes()));

    for (unsigned rr = 0; rr < two.getLatitudes().count(); rr++)
      for (unsigned cc = 0; cc < two.getLongitudes().count(); cc++)
        result->getCell(rr, cc) = one * two.getCellConst(rr, cc);

    return *result;
  }
}
