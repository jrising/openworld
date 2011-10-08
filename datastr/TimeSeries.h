#ifndef TIME_SERIES_H
#define TIME_SERIES_H

namespace openworld {
  template<class T>
  class TimeSeries : public Matrix<T> {
  protected:
    DividedRange time;
    
  public:
    TimeSeries(DividedRange time)
      : Matrix<T>(time.count(), 1),
      time(time) {
    }

    static TimeSeries<T>* loadDelimited(DividedRange time, string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      unsigned int rows, cols, count;
      list<T>* lst = Matrix<T>::loadDelimited(filepath, &rows, &cols, parser, delimiter);

      if (lst->size() != time.count())
        cout << "Data: " << lst->size() << " vs. Time: " << time.count() << endl;

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
  };
}

#endif
