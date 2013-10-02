#include "Unit.h"
#include "Units.h"

using namespace openworld;

Unit Unit::operator/(const Unit& b) const {
  if (dims == b.dims)
    return Units::none;
  else
    return *this * (b.raisedTo(-1));
}

Unit Unit::operator*(const Unit& b) const {
  if (*this == Units::none)
    return b;
  if (b == Units::none)
    return *this;

  // Below modified from Dimensions.h
  map<Unit, double> newfactors;

  if (!name.empty())
    newfactors[*this] = 1.0;
  else {
    for (map<Unit, double>::const_iterator it = factors.begin(); it != factors.end(); it++)
      newfactors[it->first] = it->second;
  }
	
  if (!b.name.empty()) {
    if (newfactors.count(b) > 0)
      newfactors[b]++;
    else
      newfactors[b] = 1.0;
  } else {
    for (map<Unit, double>::const_iterator it = b.factors.begin(); it != b.factors.end(); it++)
      if (newfactors.count(it->first) > 0)
        newfactors[it->first] += it->second;
      else
        newfactors[it->first] = it->second;
  }
      
  // Remove all elements with power 0
  map<Unit, double> newfactors2;
  for (map<Unit, double>::const_iterator it = newfactors.begin(); it != newfactors.end(); it++)
    if (it->second != 0)
      newfactors2[it->first] = it->second;

  if (newfactors2.size() == 1 && newfactors2.begin()->second == 1)
    return newfactors2.begin()->first;

  return Unit(newfactors2, dims * b.dims);
}
