/******************************************************************************\
 * Dims: standard dimensions
 ******************************************************************************
 *
\******************************************************************************/

#include "Dims.h"

using namespace openworld;

Unit Unit::none = Unit("none", Dims::none());
Unit Unit::s = Unit("s", Dims::time());
Unit Unit::m = Unit("m", Dims::distance());
Unit Unit::kg = Unit("kg", Dims::mass());
Unit Unit::mt = Unit("kg", Dims::mass());
Unit Unit::deg = Unit("deg", Dims::angle());
