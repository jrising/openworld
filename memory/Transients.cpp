/******************************************************************************\
 * Transients: objects allocated only until the next cleaning call
 ******************************************************************************
 *
\******************************************************************************/

#include "Transients.h"

using namespace openworld;

IDeletable::~IDeletable() {
  Transients::abandon(this);
}

list<IDeletable*> Transients::objects = list<IDeletable*>();
bool Transients::initialized = false;
bool Transients::cleaning = false;
