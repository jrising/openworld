#include "Stock.h"

Stock::TemporalVariable& dt() {
  return new StockFlow(this, this->unit / Units::s());
}
