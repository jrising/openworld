/******************************************************************************\
 * Inds: standard indicators
 ******************************************************************************
 *
\******************************************************************************/

#include "Inds.h"
#include <limits>

using namespace openworld;

Indicator Inds::lat = Indicator("latitude", Dims::deg(), -90, 90);
Indicator Inds::lon = Indicator("longitude", Dims::deg(), -180, 180);
Indicator Inds::unixtime = Indicator("Unix time", Dims::s(), 0, numeric_limits<double>::max());
