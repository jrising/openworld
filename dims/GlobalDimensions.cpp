/******************************************************************************\
 * GlobalDimensions: central repository of dimensions
 ******************************************************************************
 *
\******************************************************************************/

#include "GlobalDimensions.h"

using namespace openworld;

map<string, Dimensions*> GlobalDimensions::dict = map<string, Dimensions*>();
bool GlobalDimensions::initialized = false;
