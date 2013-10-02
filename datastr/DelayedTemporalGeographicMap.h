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
    DataSupplier<T>* supplier; // owns

  public:
    RowTimeGeographicMapSupplier(DividedRange latitudes, DividedRange longitudes, DataSupplier<T>* supplier)
      : latitudes(latitudes), longitudes(longitudes), supplier(supplier) {
    }

    virtual DataSupplier<GeographicMap<T>*>* clone() {
      return new RowTimeGeographicMapSupplier(latitudes, longitudes, supplier->clone());
    }

    virtual ~RowTimeGeographicMapSupplier() {
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

    DataSupplier<T>* getPrimitiveDataSupplier() {
      return supplier;
    }
  };

  template<class T>
  class DelayedGeographicMapHelper {
  protected:
    DataSupplier<GeographicMap<T>*>* supplier; // owns
    vector<GeographicMap<T>*> loaded; // owned after index loadedOwnedIndex
    unsigned loadedOwnedIndex;
    int notsaving_index;
    
  public:
    DelayedGeographicMapHelper(DataSupplier<GeographicMap<T>*>* supplier, bool saving = true)
      : supplier(supplier) {
      if (saving)
        notsaving_index = -2; // saving
      else
        notsaving_index = -1; // not saving, but none collected
      loadedOwnedIndex = 0;
    }

    DelayedGeographicMapHelper(DelayedGeographicMapHelper<T>& copy) {
      this->supplier = copy.supplier->clone();
      for (unsigned ii = 0; ii < copy.loaded.size(); ii++)
        this->loaded.push_back(copy.loaded[ii]);
      loadedOwnedIndex = copy.loaded.size();

      this->notsaving_index = copy.notsaving_index;
    }

    ~DelayedGeographicMapHelper() {
      typename vector<GeographicMap<T>*>::iterator it;
      for (it = loaded.begin() + loadedOwnedIndex; it != loaded.end(); it++)
        delete *it;

      delete supplier;
    }

    DividedRange getLongitudes() {
      return supplier->getLongitudes();
    }

    DividedRange getLatitudes() {
      return supplier->getLatitudes();
    }

    DataSupplier<T>* getPrimitiveDataSupplier() {
      return ((RowTimeGeographicMapSupplier<T>*) supplier)->getPrimitiveDataSupplier();
    }

    GeographicMap<T>& getMap(unsigned index) {
      if (notsaving_index == -2) {
        // saving
        while ((unsigned) index >= loaded.size() && !supplier->done()) {
          GeographicMap<T>* map = supplier->get();
          if (!map)
            throw runtime_error("Index outside of file");
          loaded.push_back(map);
        }
        if (supplier->done() && loaded.size() <= index)
          throw runtime_error("Index outside of file (file done)");

        return *loaded[index];
      } else {
        if (notsaving_index > -1) {
          if (index < (unsigned) notsaving_index)
            throw runtime_error("Asked for a previous index in not-saving delayed");
        
          if (index == (unsigned) notsaving_index)
            return *loaded[0];
        }

        while ((unsigned) index >= loaded.size() && !supplier->done()) {
          GeographicMap<T>* map = supplier->get();
          if (!map)
            throw runtime_error("Index outside of file");

          if (loaded.size() > 0)
            delete loaded[0];
          loaded[0] = map;
        }

        return *loaded[0];
      }
    }
  };

  template<class T>
  class DelayedTemporalGeographicMap : public TemporalGeographicMap<T> {
  protected:
    DelayedGeographicMapHelper<T> helper;
    DividedRange latitudes;
    DividedRange longitudes;

  public:
    DelayedTemporalGeographicMap(DataSupplier<GeographicMap<T>*>* supplier, DividedRange latitudes, DividedRange longitudes, DividedRange time)
      : TemporalGeographicMap<T>(NULL, time), helper(supplier), latitudes(latitudes), longitudes(longitudes) {
      this->latitudes = latitudes;
      this->longitudes = longitudes;
    }

    // takes helper (does not copy)
    DelayedTemporalGeographicMap(DelayedGeographicMapHelper<T>& helper, DividedRange latitudes, DividedRange longitudes, DividedRange time)
      : TemporalGeographicMap<T>(NULL, time), helper(helper), latitudes(latitudes), longitudes(longitudes) {
    }

    virtual TemporalGeographicMap<T>* clone() {
      return new DelayedTemporalGeographicMap<T>(helper, this->latitudes, this->longitudes, this->time);
    }

    virtual GeographicMap<T>& operator[](Measure tt) {
      int index = this->time.inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");

      return helper.getMap(index);
    }

    // debugging...
    GeographicMap<T>& getMap(unsigned index) {
      return helper.getMap(index);
    }

    virtual DividedRange getLongitudes() {
      return longitudes;
    }

    virtual DividedRange getLatitudes() {
      return latitudes;
    }

    DataSupplier<T>* getPrimitiveDataSupplier() {
      return helper.getPrimitiveDataSupplier();
    }

    static DelayedTemporalGeographicMap<T>* loadDelimited(DividedRange latitudes, DividedRange longitudes, DividedRange time, string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      // Using IRI standard ROW*TIME x COLS
      DelimitedSupplier<T>* supplier = new DelimitedSupplier<T>(filepath, parser, delimiter);
      return new DelayedTemporalGeographicMap<T>(new RowTimeGeographicMapSupplier<T>(latitudes, longitudes, supplier), latitudes, longitudes, time);
    }

    static DelayedTemporalGeographicMap<T>* loadBinary(DividedRange latitudes, DividedRange longitudes, DividedRange time, string filepath, unsigned datasize, T (*parser)(string) = NULL) {
      // Using IRI standard ROW*TIME x COLS
      BinarySupplier<T>* supplier = new BinarySupplier<T>(filepath, datasize, parser);
      return new DelayedTemporalGeographicMap<T>(new RowTimeGeographicMapSupplier<T>(latitudes, longitudes, supplier), latitudes, longitudes, time);
    }
  };
}

#endif
