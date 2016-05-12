#ifndef TIME_SERIES_H
#define TIME_SERIES_H

#include "Matrix.h"
#include "DividedRange.h"
#include "AbstractData.h"

namespace openworld {
  template<class T>
    class TimeSeries : public Matrix<T>, public AbstractTemporalData<T> {
  protected:
    DividedRange time;

  public:
    TimeSeries(DividedRange time)
      : Matrix<T>(time.count(), 1),
      time(time) {
    }

    unsigned size() const {
      return this->getRows();
    }

    virtual AbstractTemporalData<T>* clone() {
      return loadMatrixColumn(time, *this, 0);
    }

    virtual DividedRange getTimes() {
      return time;
    }

    virtual T& operator[](Measure tt) {
      return get(tt);
    }

    T& get(Measure tt) {
      int index = time.inRange(tt);
      if (index < 0)
        throw runtime_error("Time out of bounds");

      return this->values[index];
    }

    T& get(unsigned ii) {
      return this->values[ii];
    }

    vector<T> toVector() {
      vector<T> values;
      for (unsigned ii = 0; ii < this->getRows(); ii++)
        values.push_back(get(ii));

      return values;
    }

    static TimeSeries<T>* loadDelimited(DividedRange time, string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      unsigned int rows, cols, count;
      list<T>* lst = Matrix<T>::loadDelimited(filepath, &rows, &cols, parser, delimiter);

      if (lst->size() != time.count())
        cout << "Size mismatch in " << filepath << ": Data: " << lst->size() << " vs. Time: " << time.count() << endl;

      if (rows == 1)
        count = cols;
      else if (cols == 1)
        count = rows;
      else
        throw runtime_error("Timeseries has multiple columns");

      TimeSeries<T>* series = new TimeSeries<T>(time);
      series->loadIteratorInto(lst->begin(), count, 1);

      return series;
    }

    static TimeSeries<T>* loadMatrixColumn(DividedRange time, Matrix<T>& data, unsigned column) {
      TimeSeries<T>* series = new TimeSeries<T>(time);
      series->loadMatrixInto(data.subset(0, column, data.getRows(), 1));

      return series;
    }

    static TimeSeries<T>* loadVector(DividedRange time, vector<T> data) {
      TimeSeries<T>* series = new TimeSeries<T>(time);

      for (unsigned ii = 0; ii < data.size(); ii++)
        series->get(ii) = data[ii];

      return series;
    }

    void saveDelimited(string filepath, string (*formatter)(T) = NULL, char delimiter = ',') {
      ofstream file(filepath.c_str());

      if (!formatter)
        formatter = FileFormatter<T>::formatSimple;

      for (unsigned int ii = 0; ii < time.count(); ii++) {
        unsigned date = DividedRange::toReadableDate(time.getCellCenter(ii).getValue());
        file << date << "," << formatter(this->values[ii]) << endl;
      }

      file.close();
    }

  };
}

#endif
