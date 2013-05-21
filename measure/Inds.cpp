/******************************************************************************\
 * Inds: standard indicators
 ******************************************************************************
 *
\******************************************************************************/

#include "Inds.h"
#include <limits>
#include <stdexcept>
#include "Indicator.h"
#include "../dims/Dims.h"

using namespace openworld;

Indicator Indicators::lat = Indicator("latitude", Units::deg, -90, 90);
Indicator Indicators::lon = Indicator("longitude", Units::deg, -180, 180);
Indicator Indicators::unixtime = Indicator("Unix time", Units::s, 0, numeric_limits<double>::max());
