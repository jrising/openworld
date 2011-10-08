#ifndef MATRIX_H
#define MATRIX_H

namespace openmodel;

class Matrix {
 public:
  static double[,] Transpose(double[,] xx) {
    double[,] yy = new double[xx.GetLength(1), xx.GetLength(0)];
    for (int rr = 0; rr < xx.GetLength(0); rr++)
      for (int cc = 0; cc < xx.GetLength(1); cc++)
        yy[cc, rr] = xx[rr, cc];
    return yy;
  }
		
  static double[,] Multiply(double[,] xx, double[,] yy) {
    if (xx.GetLength(1) != yy.GetLength(0))
      throw new ArgumentException("Inside dimensions must match");
	
    double[,] zz = new double[xx.GetLength(0), yy.GetLength(1)];
    for (int rr = 0; rr < xx.GetLength(0); rr++)
      for (int cc = 0; cc < yy.GetLength(1); cc++) {
        double sum = 0;
        for (int ii = 0; ii < xx.GetLength(1); ii++)
          sum += xx[rr, ii] * yy[ii, cc];
        zz[rr, cc] = sum;
      }
			
    return zz;
  }
		
  static double[,] Inverse(double[,] xx) {
    if (xx.GetLength(0) == 1 && xx.GetLength(1) == 1)
      return new double[1, 1] { {1 / xx[0,0]} };
	
    if (xx.GetLength(0) == 2 && xx.GetLength(1) == 2) {
      double det = xx[0, 0] * xx[1, 1] - xx[0, 1] * xx[1, 0];
      double[,] yy = new double[2, 2];
      yy[0, 0] = xx[1, 1] / det;
      yy[0, 1] = -xx[0, 1] / det;
      yy[1, 0] = -xx[1, 0] / det;
      yy[1, 1] = xx[0, 0] / det;
      return yy;
    }
			
    throw new ArgumentException("Unknown matrix dimensions");
  }
  
  static double[,] Identity(int n) {
    double[,] xx = new double[n, n];
    for (int ii = 0; ii < n; ii++)
      xx[ii, ii] = 1;
	
    return xx;
  }
		
  static double[,] Ones(int rows, int cols) {
    double[,] xx = new double[rows, cols];
    for (int rr = 0; rr < rows; rr++)
      for (int cc = 0; cc < cols; cc++)
        xx[rr, cc] = 1;
    
    return xx;
  }
  
  static double[,] HorizontalConcat(double[,] xx, double[,] yy) {
    double[,] zz = new double[xx.GetLength(0), xx.GetLength(1) + yy.GetLength(1)];
    for (int rr = 0; rr < xx.GetLength(0); rr++)
      for (int cc = 0; cc < xx.GetLength(1); cc++)
        zz[rr, cc] = xx[rr, cc];
    
    for (int rr = 0; rr < yy.GetLength(0); rr++)
      for (int cc = 0; cc < yy.GetLength(1); cc++)
        zz[rr, cc + xx.GetLength(1)] = yy[rr, cc];
    
    return zz;
  }
		
  static double[,] GetColumn(double[,] xx, int col) {
    double[,] yy = new double[xx.GetLength(0), 1];
    for (int ii = 0; ii < xx.GetLength(0); ii++)
      yy[ii, 0] = xx[ii, col];
	
    return yy;
  }
}

#endif
