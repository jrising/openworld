/******************************************************************************\
 * AbstractData: super class for things that enter into intelligent equations
\******************************************************************************/
#ifndef ABSTRACT_DATA_H
#define ABSTRACT_DATA_H

#include <stdio.h>
#include <string>
#include <measure/Inds.h>
#include "../memory/Transients.h"
#include "../utils/ToString.h"

using namespace std;

namespace openworld {
  template <class TBase, class TNumeric, class TLogical>
  class AbstractData {
  public:
    // Overloaded Operators
    virtual TLogical& operator>=(const TNumeric& two) const = 0;
    virtual TLogical& operator>=(TBase two) const = 0;
    virtual TLogical& operator>(TBase two) const = 0;
    virtual TLogical& operator<=(const TNumeric& two) const = 0;
    virtual TLogical& operator<=(TBase two) const = 0;
    virtual TLogical& operator<(TBase two) const = 0;

    virtual TNumeric& operator*(const TNumeric& two) const = 0;
    virtual TNumeric& operator/(const TNumeric& two) const = 0;
    virtual TNumeric& dividedBy(const TNumeric& two, const TNumeric& divby0) const = 0;
    virtual TNumeric& operator+(const TNumeric& two) const = 0;
    virtual TNumeric& operator-(const TNumeric& two) const = 0;
    virtual TNumeric& operator+(TBase two) const = 0;
    virtual TNumeric& operator-(TBase two) const = 0;

    virtual TNumeric& operator*(TBase two) const = 0;
    //virtual TNumeric& operator/(TBase denom) const = 0; <- what if TBase is bool, doesn't match with /double

    virtual TNumeric& operator+=(const TNumeric& two) = 0;
    virtual TNumeric& operator*=(const TLogical& two) = 0;
    virtual TNumeric& operator/=(const TNumeric& two) = 0;
    /*virtual TNumeric& operator+=(TBase add) = 0; <- what if TBase is bool, doesn't match with .. double
    virtual TNumeric& operator-=(TBase diff) = 0;
    virtual TNumeric& operator*=(TBase mult) = 0;
    virtual TNumeric& operator/=(TBase denom) = 0;*/
  };
}

#endif
