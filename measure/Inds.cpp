/******************************************************************************\
 * Inds: standard indicators
 ******************************************************************************
 *
\******************************************************************************/

#include "Inds.h"
#include <limits>
#include <stdexcept>
#include "Indicator.h"
#include "Units.h"

using namespace openworld;

Indicator Inds::lat = LinearIndicator("latitude", Units::deg, -90, 90);
Indicator Inds::lon = LinearIndicator("longitude", Units::deg, -180, 180);
Indicator Inds::unixtime = LinearIndicator("Unix time", Units::s, 0, numeric_limits<double>::max());
