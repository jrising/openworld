#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H

#include <Matrix.h>
#include <Memory.h>

namespace openworld {
  class Matd {
  public:
    static Matrix<double>* ones(unsigned rows, unsigned cols) {
      Matrix<double>* result = mew_(Matrix<double>(rows, cols));
      for (unsigned rr = 0; rr < rows; rr++)
        for (unsigned cc = 0; cc < cols; cc++)
          result->get(rr, cc) = 1;
      
      return result;
    }

    static Matrix<double>* map(double (*func)(double), Matrix<double>* mm) {
      Matrix<double>* result = mew_(Matrix<double>(mm->getRows(), mm->getCols()));
      for (unsigned rr = 0; rr < mm->getRows(); rr++)
        for (unsigned cc = 0; cc < mm->getCols(); cc++)
          result->get(rr, cc) = func(mm->get(rr, cc));

      return result;
    }

    static Matrix<double>* cumsum(Matrix<double>* mm, bool byRow = true) {
      if (byRow) {
        Matrix<double>* result = mew_(Matrix<double>(mm->getRows(), mm->getCols()));
        for (unsigned rr = 0; rr < mm->getRows(); rr++)
          result->get(rr, 0) = mm->get(rr, 0);
        for (unsigned cc = 0; cc < mm->getCols(); cc++)
          for (unsigned rr = 1; rr < mm->getRows(); rr++)
            result->get(rr, cc) = mm->get(rr - 1, cc) + mm->get(rr, cc);

        return result;
      } else {
        Matrix<double>* result = mew_(Matrix<double>(mm->getRows(), mm->getCols()));
        for (unsigned cc = 0; cc < mm->getCols(); cc++)
          result->get(0, cc) = mm->get(0, cc);
        for (unsigned rr = 0; rr < mm->getRows(); rr++)
          for (unsigned cc = 1; cc < mm->getCols(); cc++)
            result->get(rr, cc) = mm->get(rr, cc - 1) + mm->get(rr, cc);

        return result;
      }
    }

    static Matrix<double>* transpose(Matrix<double>* mm) {
      Matrix<double>* result = mew_(Matrix<double>(mm->getCols(), mm->getRows()));
      for (unsigned rr = 0; rr < mm->getRows(); rr++)
        for (unsigned cc = 0; cc < mm->getCols(); cc++)
          result->get(cc, rr) = mm->get(rr, cc);

      return result;
    }

    static Matrix<double>* concatCols(Matrix<double>* mm1, Matrix<double>* mm2, Matrix<double>* mm3) {
      unsigned cols = mm1->getCols() + mm2->getCols() + (mm3 ? mm3->getCols() : 0);
      unsigned rows = mm1->getRows();
      if (rows != mm2->getRows() || (mm3 && rows != mm3->getRows()))
        throw runtime_error("Inconsistent row count");

      Matrix<double>* result = mew_(Matrix<double>(rows, cols));
      for (unsigned cc = 0; cc < mm1->getCols(); cc++)
        for (unsigned rr = 0; rr < mm1->getRows(); rr++)
          result->get(rr, cc) = mm1->get(rr, cc);
      for (unsigned cc = 0; cc < mm2->getCols(); cc++)
        for (unsigned rr = 0; rr < mm2->getRows(); rr++)
          result->get(rr, mm1->getCols() + cc) = mm2->get(rr, cc);
      if (mm3) {
        for (unsigned cc = 0; cc < mm3->getCols(); cc++)
          for (unsigned rr = 0; rr < mm3->getRows(); rr++)
            result->get(rr, mm1->getCols() + mm2->getCols() + cc) = mm3->get(rr, cc);
      }

      return result;
    }

    /*static Matrix<double>* concatCols(params Matrix<double>* mms, unsigned count) {
      unsigned cols = 0, rows = 0;
      for (unsigned ii = 0; ii < count; ii++) {
        cols += mms[ii]->getCols();
        if (rows == 0)
          rows = mms[ii]->getRows();
        else if (rows != mm[ii]->getRows())
          throw runtime_error("Inconsistent row count");
      }

      Matrix<double>* result = mew_(Matrix<double>(rows, cols));
      unsigned cctotal = 0;
      for (unsigned ii = 0; ii < count; ii++)
        for (unsigned cc = 0; cc < mms[ii]->getCols(); cc++) {
          for (unsigned rr = 0; rr < mms[ii]->getRows(); rr++)
            result->get(rr, cctotal) = mms[ii]->get(rr, cc);
          cctotal++;
        }

      return result;
      }*/

    static Matrix<double>* vec(Matrix<double>* mm) {
      Matrix<double>* result = mew_(Matrix<double>(mm->getRows() * mm->getCols(), 1));
      for (unsigned rr = 0; rr < mm->getRows(); rr++)
        for (unsigned cc = 0; cc < mm->getCols(); cc++)
          result->get(cc * mm->getRows() + rr, 1) = mm->get(rr, cc);

      return result;
    }
  };
}

#endif
