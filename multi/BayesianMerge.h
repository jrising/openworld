#ifndef BAYESIAN_MERGE_H
#define BAYESIAN_MERGE_H

#include <vector>
#include <multi/GaussianDistribution.h>
#include <gsl/gsl_linalg.h>

using namespace std;

namespace openworld {
  class BayesianMerge {
  public:
    // modifies means in-place to assure maximum likelihood
    static void mergeGaussianHierarchy(vector<GaussianDistribution*> lows, GaussianDistribution* high) {
      vector<double> A, b;
      for (unsigned ii = 0; ii < lows.size(); ii++) {
        for (unsigned jj = 0; jj < lows.size(); jj++) {
          if (ii == jj)
            A.push_back(1 / lows[ii]->getVariance() + 1 / high->getVariance());
          else
            A.push_back(1 / high->getVariance());
        }

        b.push_back(lows[ii]->getMean() / lows[ii]->getVariance() + high->getMean() / high->getVariance());
      }

      gsl_matrix_view Amat = gsl_matrix_view_array(A.data(), lows.size(), lows.size());
      gsl_vector_view bmat = gsl_vector_view_array(b.data(), lows.size());
      gsl_vector* x = gsl_vector_alloc(lows.size());

      int s;
     
      gsl_permutation* p = gsl_permutation_alloc(lows.size());
     
      gsl_linalg_LU_decomp(&Amat.matrix, p, &s);
      gsl_linalg_LU_solve(&Amat.matrix, p, &bmat.vector, x);
     
      double meansum = 0;
      for (unsigned ii = 0; ii < lows.size(); ii++) {
        meansum += gsl_vector_get(x, ii);
        lows[ii]->setMean(gsl_vector_get(x, ii));
      }

      high->setMean(meansum);
     
      gsl_permutation_free(p);
      gsl_vector_free(x);
    }
  };
}

#endif
