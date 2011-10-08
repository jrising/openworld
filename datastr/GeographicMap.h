/******************************************************************************\
 * GeographicMap: A collection of data about the surface of the planet
 ******************************************************************************
 * GeographicMaps are not matrices, but all of their data is in a matrix;
 *   when accessed or added, they are treated like discretely sampled values
 * XXX: Eventually, this should derive from a "ContinuousButSampled2DFunction" 
\******************************************************************************/
#ifndef GEOGRAPHIC_MAP_H
#define GEOGRAPHIC_MAP_H

#include <stdio.h>
#include <string>
#include "Matrix.h"
#include "DividedRange.h"
#include "../indicator/Inds.h"
#include "../memory/Transients.h"

using namespace std;

namespace openworld {
  class AbstractGeographicMap {
  public:
    virtual double getDouble(Measure latitude, Measure longitude) const = 0;
  };

  // XXX: This still assumes an even distribution of points (eventually need not!)
  template <class T>
  class GeographicMap : public AbstractGeographicMap {
  protected:
    DividedRange latitudes;
    DividedRange longitudes;
    const AbstractGeographicMap* source;

    GeographicMap()
      : latitudes(DividedRange(Inds::lat)), longitudes(DividedRange(Inds::lon)), source(NULL) {
    }

    GeographicMap(DividedRange latitudes, DividedRange longitudes)
      : latitudes(latitudes), longitudes(longitudes), source(NULL) {
    }

  public:
    template<class S>
    GeographicMap(const GeographicMap<S>& src)
      : latitudes(src.getLatitudes()), longitudes(src.getLongitudes()), source(&src) {
    }

    DividedRange getLatitudes() const {
      return latitudes;
    }

    DividedRange getLongitudes() const {
      return longitudes;
    }

    virtual double getDouble(Measure latitude, Measure longitude) const {
      return source->getDouble(latitude, longitude);
    }

    // refers to source
    virtual T& getCell(unsigned rr, unsigned cc) {
      throw runtime_error("getCell on GeographicMap");
    }

    // refers to source
    virtual T getCellConst(unsigned rr, unsigned cc) const {
      if (!source)
        throw runtime_error("getCellConst on uninitialized source");

      return (T) source->getDouble(latitudes.getMin() + latitudes.getWidths() * rr,
                                   longitudes.getMin() + longitudes.getWidths() * cc);
    }

    friend ostream &operator<<(ostream &out, const GeographicMap<T> &xx) {
      for (unsigned rr = 0; rr < xx.latitudes.count(); rr++) {
        for (unsigned cc = 0; cc < xx.longitudes.count(); cc++) {
          out << xx.getCellConst(rr, cc);
          if (cc < xx.longitudes.count() - 1)
            out << ", ";
        }
        out << endl;
      }

      return out;
    }


    void calcLatitudeLongitude(unsigned rr, unsigned cc, Measure& latitude, Measure& longitude) {
      latitude = latitudes.getMin() + latitudes.getWidths() * rr;
      longitude = longitudes.getMin() + longitudes.getWidths() * cc;
    }

    // in m
    void calcLocation(unsigned rr, unsigned cc, double& fromEquator, double& fromGreenwich) {
      double radiusEarth = 6371.0 * 1000;

      Measure latitude(0, Inds::lat), longitude(0, Inds::lon);
      calcLatitudeLongitude(rr, cc, latitude, longitude);

      fromGreenwich = radiusEarth * cos(M_PI * latitude.getValue() / 180) * (longitude.getValue() * M_PI / 180);
      fromEquator = radiusEarth * (latitude.getValue() * M_PI / 180);

      /* XXX: change to:
      Measure radiusEarth(6371.0 * 1000, Dims::m);

      fromGreenwich = radiusEarth * Dima::cos(latitude) * Dims::rads.convert(latitude);
      fromEquator = radiusEarth * Dims::rads.convert(latitude);
      */
    }

    // in m
    double calcDistance(unsigned rr0, unsigned cc0, unsigned rr1, unsigned cc1) {
      double fromEquator0, fromGreenwich0, fromEquator1, fromGreenwich1;

      calcLocation(rr0, cc0, fromEquator0, fromGreenwich0);
      calcLocation(rr1, cc1, fromEquator1, fromGreenwich1);

      return sqrt((fromEquator1 - fromEquator0) * (fromEquator1 - fromEquator0) +
                  (fromGreenwich1 - fromGreenwich0) * (fromGreenwich1 - fromGreenwich0));
    }

    // in m^2
    double calcArea(unsigned rr, unsigned cc) {
      double meridional = calcDistance(rr - 1, cc, rr + 1, cc) / 2;
      double zonal = calcDistance(rr, cc - 1, rr, cc + 1) / 2;
      return meridional * zonal;
    }

    T max() const {
      T result = getCellConst(0, 0);

      for (unsigned rr = 0; rr < latitudes.count(); rr++)
        for (unsigned cc = 0; cc < longitudes.count(); cc++)
          if (getCellConst(rr, cc) > result)
            result = getCellConst(rr, cc);

      return result;
    }

    T min() const {
      T result = getCellConst(0, 0);

      for (unsigned rr = 0; rr < latitudes.count(); rr++)
        for (unsigned cc = 0; cc < longitudes.count(); cc++)
          if (getCellConst(rr, cc) < result)
            result = getCellConst(rr, cc);

      return result;
    }

    // Overloaded Operators
    // include MatrixGeographicMap forms for implicit conversion

    GeographicMap<bool>& operator>=(const GeographicMap<T>& two) const;
    GeographicMap<bool>& operator>=(T two) const;

    GeographicMap<T>& operator*(T two) const;

    template<class S>
    friend GeographicMap<S>& operator*(S one, const GeographicMap<S>& two);

    GeographicMap<T>& operator*(const GeographicMap<T>& two) const;
    GeographicMap<T>& operator+(const GeographicMap<T>& two) const;
    GeographicMap<T>& operator-(const GeographicMap<T>& two) const;
    GeographicMap<T>& operator+(T two) const;
    GeographicMap<T>& operator-(T two) const;

    GeographicMap<T>& operator/(double denom) const;

    GeographicMap<T>& operator+=(const GeographicMap<T>& two);
    GeographicMap<T>& operator/=(const GeographicMap<T>& two);
    GeographicMap<T>& operator/=(double denom);
  };

  template <class T>
  class MatrixGeographicMap : public GeographicMap<T>, public IDeletable {
  protected:
    Matrix<T> values;

  public:
    MatrixGeographicMap()
      : GeographicMap<T>(DividedRange(Inds::lat), DividedRange(Inds::lon)) {
    }

    MatrixGeographicMap(DividedRange latitudes, DividedRange longitudes)
      : GeographicMap<T>(latitudes, longitudes), values(latitudes.count(), longitudes.count()) {
    }

    virtual ~MatrixGeographicMap() {
    }

    template<class S>
    MatrixGeographicMap(const GeographicMap<S>& bb)
      : GeographicMap<T>(bb.getLatitudes(), bb.getLongitudes()), values(bb.getLatitudes().count(), bb.getLongitudes().count()) {

      for (unsigned rr = 0; rr < values.getRows(); rr++)
        for (unsigned cc = 0; cc < values.getCols(); cc++)
          values.get(rr, cc) = (double) bb.getCellConst(rr, cc);
    }

    void initialize(DividedRange latitudes, DividedRange longitudes) {
      values.initialize(latitudes.count(), longitudes.count());
      this->longitudes = longitudes;
      this->latitudes = latitudes;      
    }

    virtual double getDouble(Measure latitude, Measure longitude) const {
      return values.getDouble(this->getLatitudes().inRange(latitude), this->getLongitudes().inRange(longitude));
    }

    virtual T& getCell(unsigned rr, unsigned cc) {
      return values.get(rr, cc);
    }

    virtual T getCellConst(unsigned rr, unsigned cc) const {
      return values.getConst(rr, cc);
    }

    Matrix<T>& getValues() {
      return values;
    }

    void loadConstantInto(T value) {
      values.loadConstantInto(value);
    }

    // Static Methods

    static GeographicMap<T>* loadDelimited(DividedRange latitudes, DividedRange longitudes, string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      unsigned int rows, cols;
      list<T>* lst = Matrix<T>::loadDelimited(filepath, &rows, &cols, parser, delimiter);

      cout << rows << ", " << cols << " vs. " << latitudes.count() << ", " << longitudes.count() << endl;

      MatrixGeographicMap<T>* result = new MatrixGeographicMap<T>(latitudes, longitudes);
      result->values.loadIteratorInto(lst->begin(), rows, cols);

      return result;
    }

    static GeographicMap<T>* loadTIFF(DividedRange latitudes, DividedRange longitudes, string filepath) {
      DATA_TYPE datatype = Matrix<T>::getDataType();
      tiffIO tiff((char*) filepath.c_str(), datatype);

      cout << tiff.getTotalY() << ", " << tiff.getTotalX() << " vs. " << latitudes.count() << ", " << longitudes.count() << endl;

      MatrixGeographicMap<T>* result = new MatrixGeographicMap<T>(latitudes, longitudes);
      tiff.read(0, 0, tiff.getTotalY(), tiff.getTotalX(), result->values.getValues());
      return result;
    }
  };

template <class T>
GeographicMap<bool>& GeographicMap<T>::operator>=(const GeographicMap<T>& two) const {
  if (latitudes != two.getLatitudes() || longitudes != two.getLongitudes()) {
    stringstream msg;
    msg << "space mismatch: " << latitudes << " <> " << two.getLatitudes() << " or " << longitudes << " <> " << two.getLongitudes();
    throw runtime_error(msg.str());
  }

  GeographicMap<bool>* result = tew_(MatrixGeographicMap<bool>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) >= two.getCellConst(rr, cc);

  return *result;
}

template <class T>
GeographicMap<bool>& GeographicMap<T>::operator>=(T two) const {
  GeographicMap<bool>* result = tew_(MatrixGeographicMap<bool>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) >= two;

  return *result;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator*(T two) const {
  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) * two;

  return *result;
}

template <class T>
GeographicMap<T>& operator*(T one, const GeographicMap<T>& two) {
  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(two.latitudes, two.longitudes));

  for (unsigned rr = 0; rr < two.latitudes.count(); rr++)
    for (unsigned cc = 0; cc < two.longitudes.count(); cc++)
      result->getCell(rr, cc) = one * two.getCellConst(rr, cc);

  return *result;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator*(const GeographicMap<T>& two) const {
  if (latitudes != two.getLatitudes() || longitudes != two.getLongitudes()) {
    throw runtime_error("space mismatch");
  }

  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) * two.getCellConst(rr, cc);

  return *result;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator+(const GeographicMap<T>& two) const {
  if (latitudes != two.getLatitudes() || longitudes != two.getLongitudes()) {
    throw runtime_error("space mismatch");
  }

  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) + two.getCellConst(rr, cc);

  return *result;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator-(const GeographicMap<T>& two) const {
  if (latitudes != two.getLatitudes() || longitudes != two.getLongitudes()) {
    throw runtime_error("space mismatch");
  }

  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) - two.getCellConst(rr, cc);

  return *result;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator+(T two) const {
  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) + two;

  return *result;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator-(T two) const {
  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) - two;

  return *result;
}


template <class T>
GeographicMap<T>& GeographicMap<T>::operator/(double denom) const {
  GeographicMap<T>* result = tew_(MatrixGeographicMap<T>(latitudes, longitudes));

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      result->getCell(rr, cc) = getCellConst(rr, cc) / denom;

  return *result; 
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator+=(const GeographicMap<T>& two) {
  if (latitudes != two.getLatitudes() || longitudes != two.getLongitudes()) {
    throw runtime_error("space mismatch");
  }

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      getCell(rr, cc) += two.getCellConst(rr, cc);

  return *this;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator/=(const GeographicMap<T>& two) {
  if (latitudes != two.getLatitudes() || longitudes != two.getLongitudes()) {
    throw runtime_error("space mismatch");
  }

  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      getCell(rr, cc) /= two.getCellConst(rr, cc);

  return *this;
}

template <class T>
GeographicMap<T>& GeographicMap<T>::operator/=(double denom) {
  for (unsigned rr = 0; rr < latitudes.count(); rr++)
    for (unsigned cc = 0; cc < longitudes.count(); cc++)
      getCell(rr, cc) /= denom;

  return *this;
}
}

#endif
