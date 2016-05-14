#ifndef ABSTRACT_COLLECTION_H
#define ABSTRACT_COLLECTION_H

#include "AbstractData.h"
#include "GeographicMap.h"
#include "TimeSeries.h"
#include "Matrix.h"
#include <measure/MultiMeasure.h>
#include <vector>

using namespace std;

namespace openworld {
  template <class T>
    class AbstractCollection : AbstractData<T, AbstractCollection<T>, AbstractCollection<bool> >, public IDeletable {
  protected:
    vector<T> data;

  public:
    AbstractCollection() {
    }

  AbstractCollection(vector<T> clone)
    : data(clone) {
    }

    AbstractCollection(AbstractCollection<T>& copy)
      : data(copy.data) {
    }

    template<class S>
      AbstractCollection(AbstractCollection<S>& copy) {
      for (unsigned ii = 0; ii < copy.length(); ii++)
        data.push_back((T) copy.getConst(ii));
    }

    T& operator[](unsigned ii) {
      return data[ii];
    }

    T getConst(unsigned ii) const {
      return data[ii];
    }

    unsigned length() const {
      return data.size();
    }

    void add(T datum) {
      data.push_back(datum);
    }

    // Overloaded Operators
    virtual AbstractCollection<bool>& operator>=(const AbstractCollection<T>& two) const;
    virtual AbstractCollection<bool>& operator>=(T two) const;
    virtual AbstractCollection<bool>& operator>(T two) const;
    virtual AbstractCollection<bool>& operator<=(const AbstractCollection<T>& two) const;
    virtual AbstractCollection<bool>& operator<=(T two) const;
    virtual AbstractCollection<bool>& operator<(T two) const;

    virtual AbstractCollection<T>& operator*(const AbstractCollection<T>& two) const;
    virtual AbstractCollection<T>& operator/(const AbstractCollection<T>& two) const;
    virtual AbstractCollection<T>& dividedBy(const AbstractCollection<T>& two, const AbstractCollection<T>& divby0) const;
    virtual AbstractCollection<T>& operator+(const AbstractCollection<T>& two) const;
    virtual AbstractCollection<T>& operator-(const AbstractCollection<T>& two) const;
    virtual AbstractCollection<T>& operator+(T two) const;
    virtual AbstractCollection<T>& operator-(T two) const;
    virtual AbstractCollection<T>& operator*(T two) const;
    virtual AbstractCollection<T>& operator/(T two) const;

    virtual AbstractCollection<T>& operator+=(const AbstractCollection<T>& two);
    virtual AbstractCollection<T>& operator*=(const AbstractCollection<bool>& two);
    virtual AbstractCollection<T>& operator/=(const AbstractCollection<T>& two);

    template<class S>
    friend AbstractCollection<S>& operator*(S one, const AbstractCollection<S>& two);
    template<class S>
    friend AbstractCollection<S>& operator+(S one, const AbstractCollection<S>& two);
    template<class S>
    friend AbstractCollection<S>& operator-(S one, const AbstractCollection<S>& two);
    friend AbstractCollection<double>& operator*(double one, const AbstractCollection<bool>& two);

    static AbstractCollection<double>* loadMapPoints(GeographicMap<T>& map, MultiMeasure latitudes, MultiMeasure longitudes) {
      AbstractCollection<T>* collection = new AbstractCollection<T>();
      for (unsigned ii = 0; ii < latitudes.getLength(); ii++) {
        T values = map.getDouble(latitudes[ii], longitudes[ii]);
        collection->data.push_back(values);
      }

      return collection;
    }
  };

  template <class T>
    AbstractCollection<bool>& AbstractCollection<T>::operator>=(const AbstractCollection<T>& two) const {
    vector<bool> result;
    for (typename vector<T>::const_iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        result.push_back(*it1 >= *it2);

    return *tew_(AbstractCollection<bool>(result));
  }

  template <class T>
    AbstractCollection<bool>& AbstractCollection<T>::operator>=(T two) const {
    vector<bool> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it >= two);

    return *tew_(AbstractCollection<bool>(result));
  }

  template <class T>
    AbstractCollection<bool>& AbstractCollection<T>::operator>(T two) const {
    vector<bool> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it > two);

    return *tew_(AbstractCollection<bool>(result));
  }

  template <class T>
    AbstractCollection<bool>& AbstractCollection<T>::operator<=(const AbstractCollection<T>& two) const {
    vector<bool> result;
    for (typename vector<T>::const_iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        result.push_back(*it1 <= *it2);

    return *tew_(AbstractCollection<bool>(result));
  }

  template <class T>
    AbstractCollection<bool>& AbstractCollection<T>::operator<=(T two) const {
    vector<bool> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it <= two);

    return *tew_(AbstractCollection<bool>(result));
  }

  template <class T>
    AbstractCollection<bool>& AbstractCollection<T>::operator<(T two) const {
    vector<bool> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it < two);

    return *tew_(AbstractCollection<bool>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator*(const AbstractCollection<T>& two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        result.push_back(*it1 * *it2);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator/(const AbstractCollection<T>& two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        result.push_back(*it1 / *it2);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::dividedBy(const AbstractCollection<T>& two, const AbstractCollection<T>& divby0) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        for (typename vector<T>::const_iterator it3 = divby0.data.begin(); it3 != divby0.data.end(); ++it3) {
          if (*it2 == 0)
            result.push_back(*it3);
          else
            result.push_back(*it1 / *it2);
        }

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator+(const AbstractCollection<T>& two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        result.push_back(*it1 + *it2);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator-(const AbstractCollection<T>& two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        result.push_back(*it1 - *it2);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator+(T two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it + two);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator-(T two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it - two);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator*(T two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it * two);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator/(T two) const {
    vector<T> result;
    for (typename vector<T>::const_iterator it = data.begin(); it != data.end(); ++it)
        result.push_back(*it / two);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator+=(const AbstractCollection<T>& two) {
    for (typename vector<T>::iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        *it1 = *it1 + *it2;

    return *this;
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator*=(const AbstractCollection<bool>& two) {
    for (unsigned ii = 0; ii < length(); ii++)
      data[ii] = data[ii] * two.getConst(ii);

    return *this;
  }

  template <class T>
    AbstractCollection<T>& AbstractCollection<T>::operator/=(const AbstractCollection<T>& two) {
    for (typename vector<T>::iterator it1 = data.begin(); it1 != data.end(); ++it1)
      for (typename vector<T>::const_iterator it2 = two.data.begin(); it2 != two.data.end(); ++it2)
        *it1 = *it1 / *it2;

    return *this;
  }

  template <class T>
    AbstractCollection<T>& operator*(T one, const AbstractCollection<T>& two) {
    vector<T> result;
    for (typename vector<T>::const_iterator it = two.data.begin(); it != two.data.end(); ++it)
        result.push_back(one * *it);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& operator+(T one, const AbstractCollection<T>& two) {
    vector<T> result;
    for (typename vector<T>::const_iterator it = two.data.begin(); it != two.data.end(); ++it)
        result.push_back(one + *it);

    return *tew_(AbstractCollection<T>(result));
  }

  template <class T>
    AbstractCollection<T>& operator-(T one, const AbstractCollection<T>& two) {
    vector<T> result;
    for (typename vector<T>::const_iterator it = two.data.begin(); it != two.data.end(); ++it)
        result.push_back(one - *it);

    return *tew_(AbstractCollection<T>(result));
  }

  template<class T>
    class TemporalAbstractCollection : public AbstractTemporalData<AbstractCollection<T> >, public IDeletable {
  protected:
    vector<vector<T> > data;
    DividedRange time;

  public:
  TemporalAbstractCollection(Indicator timeind)
    : time(timeind) {}

  TemporalAbstractCollection(DividedRange time)
    : time(time) {}

  TemporalAbstractCollection(DividedRange time, vector<vector<T> >& copy)
    : time(time) {
      for (typename vector<vector<T> >::iterator it = copy.begin(); it != copy.end(); ++it)
        data.push_back(*it);
    }

    virtual TemporalAbstractCollection<T>* clone() {
      return new TemporalAbstractCollection<T>(time, data);
    }

    virtual DividedRange getTimes() {
      return time;
    }

    virtual AbstractCollection<T>& operator[](Measure tt) {
      AbstractCollection<T>* collection = tew_(AbstractCollection<T>());
      for (typename vector<vector<T> >::iterator it = data.begin(); it != data.end(); ++it)
        collection->add((*it)[time.inRange(tt)]);

      return *collection;
    }

    TimeSeries<T>& getSingle(unsigned ii) {
      TimeSeries<T>* series = TimeSeries<T>::loadVector(time, data[ii]);
      return *Transients::track(series);
    }

    void add(Measure tt, AbstractCollection<T>& collection) {
      if (time == DividedRange(0, 0, 0, time.getIndicator())) {
        time = DividedRange(tt.getValue(), tt.getValue(), 1, time.getIndicator());
      } else {
        time = DividedRange(time.getMin().getValue(), tt.getValue(), (time.getMax() - time.getMin()).getValue() / data[0].size() + 1, time.getIndicator());
      }

      for (unsigned ii = 0; ii < collection.length(); ii++) {
        vector<T>& datum = data[ii];
        datum.push_back(collection[ii]);
      }
    }

    virtual TemporalAbstractCollection<T>& operator-(const TemporalAbstractCollection<T>& two) const;

    virtual TemporalAbstractCollection<T>& cumsum() const {
      vector<vector<T> > result;

      for (typename vector<vector<T> >::const_iterator outerit = data.begin(); outerit != data.end(); ++outerit) {
        vector<T> subresult;
        double sum = 0;
        for (typename vector<T>::const_iterator it = outerit->begin(); it != outerit->end(); ++outerit) {
          sum += *it;
          subresult.push_back(sum);
        }

        result.push_back(subresult);
      }

      return *tew_(TemporalAbstractCollection<T>(time, result));
    }

    static TemporalAbstractCollection<T>* loadMapPoints(TemporalGeographicMap<T>& map, MultiMeasure latitudes, MultiMeasure longitudes) {
      TemporalAbstractCollection<T>* collection = new TemporalAbstractCollection<T>(map.getTimes());
      for (unsigned ii = 0; ii < latitudes.getLength(); ii++) {
        TimeSeries<T>* values = map.getTimeSeries(latitudes[ii], longitudes[ii]);
        collection->data.push_back(values->toVector());
      }

      return collection;
    }

    static TemporalAbstractCollection<T>* loadDelimitedPoints(DividedRange time, string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      Matrix<T>* values = Matrix<T>::loadDelimited(filepath, parser, delimiter);

      TemporalAbstractCollection<T>* collection = new TemporalAbstractCollection<T>(time);
      for (unsigned cc = 0; cc < values->getCols(); cc++) {
        TimeSeries<T>* series = TimeSeries<T>::loadMatrixColumn(time, *values, cc);
        collection->data.push_back(series->toVector());
      }

      return collection;
    }

    static TemporalAbstractCollection<T>* loadConstantPoints(DividedRange time, T values[], unsigned length) {
      TemporalAbstractCollection<T>* collection = new TemporalAbstractCollection<T>(time);
      for (unsigned ii = 0; ii < length; ii++) {
        TimeSeries<T>* series = new TimeSeries<T>(time);
        series->loadConstantInto(values[ii]);
        collection->data.push_back(series->toVector());
      }

      return collection;
    }
  };

  template <class T>
    TemporalAbstractCollection<T>& TemporalAbstractCollection<T>::operator-(const TemporalAbstractCollection<T>& two) const {
    vector<vector<T> > result;

    for (typename vector<vector<T> >::const_iterator outerit1 = data.begin(); outerit1 != data.end(); ++outerit1)
      for (typename vector<vector<T> >::const_iterator outerit2 = two.data.begin(); outerit2 != two.data.end(); ++outerit2) {
        vector<T> subresult;
        for (typename vector<T>::const_iterator it1 = outerit1->begin(); it1 != outerit1->end(); ++outerit1)
          for (typename vector<T>::const_iterator it2 = outerit2->begin(); it2 != outerit2->end(); ++outerit2)
            subresult.push_back(*it1 - *it2);

        result.push_back(subresult);
      }

    return *tew_(TemporalAbstractCollection<T>(time, result));
  }
}

#endif
