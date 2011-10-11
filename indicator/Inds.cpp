/******************************************************************************\
 * Inds: standard indicators
 ******************************************************************************
 *
\******************************************************************************/

#include "Inds.h"

using namespace openworld;

Indicator Inds::lat = Indicator("latitude", Dims::deg());
Indicator Inds::lon = Indicator("longitude", Dims::deg());
Indicator Inds::unixtime = Indicator("Unix time", Dims::s());
