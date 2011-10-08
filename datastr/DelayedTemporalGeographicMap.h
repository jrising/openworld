#ifndef DELAYED_TEMPORAL_GEOGRAPHIC_MAP_H
#define DELAYED_TEMPORAL_GEOGRAPHIC_MAP_H

#include <list>
#include "TemporalGeographicMap.h"
#include "DataSupplier.h"
#include "../utils/ToString.h"

using namespace std;

namespace openworld {
  template<class T>
  class RowTimeGeographicMapSupplier : public DataSupplier<GeographicMap<T>*> {
  protected:
    DividedRange latitudes;
    DividedRange longitudes;
    DataSupplier<T>* supplier;

  public:
    RowTimeGeographicMapSupplier(DividedRange latitudes, DividedRange longitudes, DataSupplier<double>* supplier)
      : latitudes(latitudes), longitudes(longitudes), supplier(supplier) {
    }

    ~RowTimeGeographicMapSupplier() {
      delete supplier;
    }

    virtual GeographicMap<T>* get() {
      if (supplier->done())
        return NULL;

      MatrixGeographicMap<T>* result = new MatrixGeographicMap<T>(latitudes, longitudes);

      for (unsigned rr = 0; rr < result->getValues().getRows(); rr++)
        for (unsigned cc = 0; cc < result->getValues().getCols(); cc++)
          result->getValues().get(rr, cc) = supplier->get();

      DelimitedSupplier<T>* delsup;
      if ((delsup = dynamic_cast<DelimitedSupplier<T>*>(supplier))) {
        if (delsup->getCols() != longitudes.count()) {
          stringstream msg;
          msg << "Columns mismatch: " << delsup->getCols() << " <> " << longitudes.count();
          throw runtime_error(msg.str());
        }
        if (delsup->getRows() % latitudes.count() != 0)
          throw runtime_error("Rows mismatch: " + ToString::base10(delsup->getRows()) + " % " + ToString::base10(latitudes.count()));
      }

      return result;
    }

    virtual int length() {
      return -1;
    }

    virtual bool done() {
      return supplier->done();
    }
  };

  template<class T>
  class DelayedGeographicMapHelper {
  protected:
    DataSupplier<GeographicMap<T>*>* supplier;
    vector<GeographicMap<T>*> loaded;
    
  public:
    DelayedGeographicMapHelper(DataSupplier<GeographicMap<T>*>* supplier)
      : supplier(supplier) {
    }

    ~DelayedGeographicMapHelper() {
      typename vector<GeographicMap<T>*>::iterator it;
      for (it = loaded.begin(); it != loaded.end(); it++)
        delete *it;

      delete supplier;
    }

    GeographicMap<T>& getMap(unsigned index) {
      cout << "getMap " << index << endl;
      while ((unsigned) index >= loaded.size() && !supplier->done()) {
        GeographicMap<T>* map = supplier->get();
        if (!map)
          throw runtime_error("Index outside of file");
        loaded.push_back(map);
      }
      
      return *loaded[index];
    }
  };

  template<class T>
  class DelayedTemporalGeographicMap : public TemporalGeographicMap<T> {
  protected:
    DelayedGeographicMapHelper<T> helper;

  public:
    DelayedTemporalGeographicMap(DataSupplier<GeographicMap<T>*>* supplier, DividedRange time)
      : TemporalGeographicMap<T>(NULL, time), helper(supplier) {
    }

    virtual GeographicMap<T>& operator[](Measure tt) {
      int index = this->time.inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");

      return helper.getMap(index);
    }

    static DelayedTemporalGeographicMap<T>* loadDelimited(DividedRange latitudes, DividedRange longitudes, DividedRange time, string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      // Using IRI standard ROW*TIME x COLS
      DelimitedSupplier<T>* supplier = new DelimitedSupplier<T>(filepath, parser, delimiter);
      return new DelayedTemporalGeographicMap<T>(new RowTimeGeographicMapSupplier<T>(latitudes, longitudes, supplier), time);
    }
  };
}

#endif
