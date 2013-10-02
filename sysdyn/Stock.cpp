#include "Stock.h"
#include <measure/Units.h>
#include <memory/Transients.h>

namespace openworld {
  double Stock::deltat = .1;

  TemporalVariable& Stock::dt() {
    return *tew_(StockFlow(*this, this->unit / Units::s, holder));
  }
}
