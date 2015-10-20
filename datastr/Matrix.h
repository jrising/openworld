#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <stdexcept>
#ifndef SKIP_TIFF
#include "../geotiff/tiffIO.h"
#endif
#include "FileFormats.h"
#include "../memory/Transients.h"

using namespace std;

namespace openworld {
  class MatrixAbstract {
  protected:
    unsigned int rows;
    unsigned int cols;

  public:
    MatrixAbstract() {
      rows = cols = 0;
    }

    MatrixAbstract(unsigned int rows, unsigned int cols) {
      this->rows = rows;
      this->cols = cols;
    }

    MatrixAbstract(const MatrixAbstract& copy) {
      rows = copy.rows;
      cols = copy.cols;
    }

    virtual ~MatrixAbstract() {}

    virtual double getDouble(unsigned int rr, unsigned int cc) const {
      throw runtime_error("Not implemented");
    }

    virtual long getLong(unsigned int rr, unsigned int cc) const {
      throw runtime_error("Not implemented");
    }

    friend ostream &operator<<(ostream &out, const MatrixAbstract &xx) {
      for (unsigned rr = 0; rr < xx.rows; rr++) {
        for (unsigned cc = 0; cc < xx.cols; cc++) {
          out << xx.getDouble(rr, cc);
          if (cc < xx.cols - 1)
            out << ", ";
        }
        out << endl;
      }

      return out;
    }
  };

  template <class T>
  class Matrix : public MatrixAbstract, public IDeletable {
  protected:
    T* values; // owns

    void forget() {
      this->rows = this->cols = 0;
      values = NULL;
    }

  public:
    Matrix() {
      values = NULL;
    }

    Matrix(unsigned int rows, unsigned int cols, T** values = NULL)
      : MatrixAbstract(rows, cols) {
      this->values = new T[rows * cols];

      if (values != NULL) {
        for (unsigned int rr = 0; rr < rows; rr++)
          for (unsigned int cc = 0; cc < cols; cc++)
            this->values[rr * cols + cc] = values[rr][cc];
      }
    }

    Matrix(MatrixAbstract& copy)
      : MatrixAbstract(copy) {
      values = new T[rows * cols];
      for (unsigned int rr = 0; rr < rows; rr++)
        for (unsigned int cc = 0; cc < cols; cc++)
          values[rr * cols + cc] = (T) copy.getDouble(rr, cc);
    }

    virtual ~Matrix() {
      if (values)
        delete[] values;
    }

    virtual list<IDeletable*> getContainedPointers() const {
      return list<IDeletable*>();
    }

    void initialize(unsigned rows, unsigned cols) {
      this->rows = rows;
      this->cols = cols;

      if (values)
        delete values;

      values = new T[rows * cols];
    }

    T* transfer() {
      T* data = values;
      forget();
      return data;
    }

    int loadDelimitedInto(string filepath, T (*parser)(string) = NULL, char delimiter = ',', int r0 = 0, int c0 = 0) {
#ifndef SKIP_GETLINE
      ifstream file(filepath);
      string line;

      if (!parser)
        parser = FileParser<T>::parseSimple;

      int rr = r0;
      while (file.good()) {
        getline((istream) file, line);
        stringstream linestream(line);
        string item;

        int cc = c0;
        while (linestream.good())
          getline((istream) linestream, item, delimiter);
          values[rr * rows + cc] = parser(item);

        rr++;
      }

      return rr;
#else
      throw runtime_error("loadDelimitedInto has not been implemented without getline");
#endif
    }

    // fills in, across each row
    void loadIteratorInto(typename list<T>::iterator iter, unsigned int rows, unsigned int cols, int r0 = 0, int c0 = 0) {
      unsigned rr = 0, cc = 0;

      //typename list<T>::iterator iter = lst.begin();
      while ((rr + r0) * this->cols < this->rows * this->cols && rr * cols < rows * cols) {
        for (cc = 0; cc < cols && cc + c0 < this->cols; cc++, iter++)
          values[(rr + r0) * this->cols + (cc + c0)] = *iter;
        while (cc++ < cols) // data can't be included
          iter++;
        rr++;
      }
    }

    void loadConstantInto(T value, unsigned rows = 0, unsigned cols = 0, unsigned r0 = 0, unsigned c0 = 0) {
      if (rows == 0)
        rows = this->rows;
      if (cols == 0)
        cols = this->cols;

      for (unsigned rr = 0; rr < rows; rr++)
        for (unsigned cc = 0; cc < cols; cc++)
          values[(rr + r0) * this->cols + (cc + c0)] = value;
    }

    virtual double getDouble(unsigned int rr, unsigned int cc) const {
      return (double) values[rr * cols + cc];
    }

    virtual long getLong(unsigned int rr, unsigned int cc) const {
      return (long) values[rr * cols + cc];
    }

    T& get(unsigned rr, unsigned cc) {
      return values[rr * cols + cc];
    }

    T getConst(unsigned rr, unsigned cc) const {
      return values[rr * cols + cc];
    }

    T* getValues() {
      return values;
    }

    unsigned getRows() const {
      return rows;
    }

    unsigned getCols() const {
      return cols;
    }

    void saveDelimited(string filepath, string (*formatter)(T) = NULL, char delimiter = ',') {
      ofstream file(filepath.c_str());

      if (!formatter)
        formatter = FileFormatter<T>::formatSimple;

      for (unsigned int ii = 0; ii < rows; ii++) {
        for (unsigned int jj = 0; jj < cols; jj++) {
          file << formatter(values[ii * cols + jj]);
          if (jj < cols - 1)
            file << delimiter;
        }
        file << endl;
      }

      file.close();
    }

#ifndef SKIP_TIFF
    void saveTIFF(string filename, string tmplfile) {
      DATA_TYPE datatype = getDataType();
      tiffIO tmpl((char*) tmplfile.c_str(), datatype);

      tiffIO tiff((char*) filename.c_str(), datatype, tmpl.getNodata(), tmpl);
      tiff.write(0, 0, tiff.getTotalY(), tiff.getTotalX(), values);
    }
#endif

    Matrix<T>& subset(unsigned r0, unsigned c0, unsigned rows, unsigned cols) {
      Matrix<T>* result = tew_(Matrix<T>(rows, cols));

      for (unsigned rr = r0; rr - r0 < rows; rr++)
        for (unsigned cc = c0; cc - c0 < cols; cc++)
          result->get(rr - r0, cc - c0) = getConst(rr, cc);

      return *result;
    }

    // Overloaded Operators

    virtual Matrix<T>& operator*(T two) const {
      Matrix<T>* result = tew_(Matrix<T>(rows, cols));

      for (unsigned rr = 0; rr < rows; rr++)
        for (unsigned cc = 0; cc < cols; cc++)
          result->get(rr, cc) = getConst(rr, cc) * two;

      return *result;
    }

    Matrix<T>& operator*=(T two) {
      for (unsigned rr = 0; rr < rows; rr++)
        for (unsigned cc = 0; cc < cols; cc++)
          get(rr, cc) *= two;

      return *this;
    }

    Matrix<bool>& operator>=(Matrix<T>& two) {
      if (rows != two.getRows() || cols != two.getCols())
        throw runtime_error("Dimensions mismatch");

      Matrix<bool>* result = tew_(Matrix<bool>(rows, cols));

      for (unsigned rr = 0; rr < rows; rr++)
        for (unsigned cc = 0; cc < cols; cc++)
          result->get(rr, cc) = get(rr, cc) >= two.get(rr, cc);

      return *result;
    }

    Matrix<bool>& operator>=(T two) {
      Matrix<bool>* result = tew_(Matrix<bool>(rows, cols));

      for (unsigned rr = 0; rr < rows; rr++)
        for (unsigned cc = 0; cc < cols; cc++)
          result->get(rr, cc) = get(rr, cc) >= two;

      return *result;
    }

    // Static Methods

    static list<T>* loadDelimited(string filepath, unsigned int* rows, unsigned int* cols, T (*parser)(string) = NULL, char delimiter = ',') {
      ifstream file(filepath.c_str());
      if (!file.good())
        throw runtime_error("Cannot open file");

      string line;

      if (!parser)
        parser = FileParser<T>::parseSimple;

      list<T>* result = new list<T>();
      *rows = 0;
      *cols = 0;
      while (file.good()) {
        getline(file, line);
        istringstream linestream(line);
        string item;

        unsigned int rowcols = 0;
        while (linestream.good()) {
          getline(linestream, item, delimiter);
          result->push_back(parser(item));
          rowcols++;
        }

        if (*cols == 0)
          *cols = rowcols;
        else if (*cols != rowcols) {
          if (!file.good())
            break;

          stringstream expected, actual, therow;
          expected << *cols;
          actual << rowcols;
          therow << *rows;
          cout << "Inconsistent row lengths: " + expected.str() + " <> " + actual.str() + " at " + therow.str();
          throw new runtime_error("Inconsistent row lengths: " + expected.str() + " <> " + actual.str() + " at " + therow.str());
        }

        (*rows)++;
      }

      return result;
    }

    static Matrix<T>* loadDelimited(string filepath, T (*parser)(string) = NULL, char delimiter = ',') {
      unsigned int rows, cols;
      list<T>* lst = loadDelimited(filepath, &rows, &cols, parser, delimiter);

      Matrix<T>* matrix = new Matrix<T>(rows, cols);
      matrix->loadIteratorInto(lst->begin(), rows, cols);
      return matrix;
    }

#ifndef SKIP_TIFF
    static Matrix<T>* loadTIFF(string filepath) {
      DATA_TYPE datatype = getDataType();
      tiffIO tiff((char*) filepath.c_str(), datatype);
      Matrix<T>* result = new Matrix<T>(tiff.getTotalY(), tiff.getTotalX());
      tiff.read(0, 0, tiff.getTotalY(), tiff.getTotalX(), result->values);
      return result;
    }

    static DATA_TYPE getDataType() {
      T dummy = 0;
      short testshort = 0;
      long testlong = 0;
      float testfloat = 0.0;
      double testdouble = 0.0;

      if (typeid(dummy) == typeid(testshort))
        return SHORT_TYPE;
      if (typeid(dummy) == typeid(testlong))
        return LONG_TYPE;
      if (typeid(dummy) == typeid(testfloat))
        return FLOAT_TYPE;
      if (typeid(dummy) == typeid(testdouble))
        return DOUBLE_TYPE;
      throw runtime_error("Unknown matrix type");
    }
#endif
  };
}

#endif
