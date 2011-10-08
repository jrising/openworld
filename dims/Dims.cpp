/******************************************************************************\
 * Dims: standard dimensions
 ******************************************************************************
 *
\******************************************************************************/

#include "Dims.h"

using namespace openworld;

Dimensions& Dims::s = GlobalDimensions::get("s");
Dimensions& Dims::m = GlobalDimensions::get("m");
Dimensions& Dims::kg = GlobalDimensions::get("kg");
Dimensions& Dims::deg = GlobalDimensions::get("deg");
