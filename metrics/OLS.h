#ifndef OLS_H
#define OLS_H

#include "../datastr/Matrix.h"

namespace openworld {
  class OLS {
  public:
    OLS() {
    }
		
    /*static Matrix<double> PointsRegression(Matrix<double> points) {
      Matrix<double> xxs = AddConstant(Matrix<double>::GetColumn(points, 0));
      Matrix<double> yy = Matrix<double>::GetColumn(points, 1);
      
      return CalcCoefficients(yy, xxs);
    }
		
    static double PointsRSqr(Matrix<double> points) {
      Matrix<double> betas = PointsRegression(points);
      Matrix<double> xxs = AddConstant(Matrix<double>::GetColumn(points, 0));
      return CalcRSqr(Matrix<double>::GetColumn(points, 1), CalcEstimates(betas, xxs));
    }
		
    static Matrix<double> AddConstant(Matrix<double> xxs) {
      return Matrix<double>::HorizontalConcat(Matrix<double>::Ones(xxs.GetLength(0), 1), xxs);
    }
	
    static Matrix<double> CalcCoefficients(Matrix<double> yy, Matrix<double> xxs) {
      return Matrix<double>::Multiply(Matrix<double>::Inverse(Matrix<double>::Multiply(Matrix<double>::Transpose(xxs), xxs)), Matrix<double>::Multiply(Matrix<double>::Transpose(xxs), yy));
    }
  
    static Matrix<double> CalcEstimates(Matrix<double> betas, Matrix<double> xxs) {
      return Matrix<double>::Multiply(xxs, betas);
      }*/
		
    static double calcRSqr(Matrix<double>* yy, Matrix<double>* yyhat) {
      double numer = 0, denom = 0;
      for (unsigned ii = 0; ii < yy->getRows(); ii++) {
        double diff = yy->get(ii, 0) - yyhat->get(ii, 0);
        numer += diff * diff;
        denom += yy->get(ii, 0) * yy->get(ii, 0);
      }
			
      return 1.0 - numer / denom;
    }
  };
}

#endif
