#ifndef GAUSSIAN_DISTRIBUTION_H
#define GAUSSIAN_DISTRIBUTION_H

#include "Distribution.h"
#include "../memory/Transients.h"

namespace openworld {
  class GaussianDistribution : public ContinuousDistribution {
  protected:
    double mean;
    double variance;
    int seed;
    
  public:
    GaussianDistribution(double mean, double variance) {
      this->mean = mean;
      this->variance = variance;
      seed = 0;
    }

    double getMean() {
      return mean;
    }

    void setMean(double mean) {
      this->mean = mean;
    }

    double getVariance() {
      return variance;
    }

    virtual double getMode() {
      return mean;
    }

    virtual double getMaxy() {
      return mean + sqrt(variance) * 3;
    }
    
    virtual Distribution& merge(Distribution& other, bool flippable = true) {
      GaussianDistribution* og = dynamic_cast<GaussianDistribution*>(&other);
      if (og == NULL) {
        if (!flippable)
          throw runtime_error("Unknown distribution to combine with Gaussian.");
        
        return other.merge(*this, false);
      }

      double combvar = variance * og->variance / (variance + og->variance);
      double combmean = (mean * og->variance + og->mean * variance) / (variance + og->variance);
      return *tew_(GaussianDistribution(combmean, combvar));
    }

    virtual void mergePurge() {}

    virtual double random(unsigned seed) {
      if (this->seed == 0)
        this->seed = (int) seed;
      
      return mean + sqrt(variance) * r8_normal_01(&this->seed);
    }

    virtual double pdf(double x) {
      return exp(-(x - mean)*(x - mean) / (2*variance)) / sqrt(2*M_PI*variance);
    }

  private:
    /*
  Purpose:

    R8_NORMAL_01 returns a unit pseudonormal R8.

  Discussion:

    The standard normal probability distribution function (PDF) has 
    mean 0 and standard deviation 1.

    Because this routine uses the Box Muller method, it requires pairs
    of uniform random values to generate a pair of normal random values.
    This means that on every other call, the code can use the second
    value that it calculated.

    However, if the user has changed the SEED value between calls,
    the routine automatically resets itself and discards the saved data.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    10 June 2010

  Author:

    John Burkardt

  Parameters:

    Input/output, int *SEED, a seed for the random number generator.

    Output, double R8_NORMAL_01, a normally distributed random value.
    */
    double r8_normal_01 (int *seed) {
#define R8_PI 3.141592653589793

      double r1;
      double r2;
      static int seed2 = 0;
      static int used = 0;
      double x;
      static double y = 0.0;
      /*
        On odd numbered calls, generate two uniforms, create two normals,
        return the first normal and its corresponding seed.
      */
      if ((used % 2) == 0) {
        r1 = r8_uniform_01(seed);
        seed2 = *seed;
        r2 = r8_uniform_01(&seed2);

        x = sqrt(-2.0 * log(r1)) * cos(2.0 * R8_PI * r2);
        y = sqrt(-2.0 * log(r1)) * sin(2.0 * R8_PI * r2);
      }
      /*
        On odd calls, return the second normal and its corresponding seed.
      */
      else {
        *seed = seed2;
        x = y;
      }

      used = used + 1;

      return x;
#undef R8_PI
    }

    /*
  Purpose:

    R8_UNIFORM_01 returns a unit pseudorandom R8.

  Discussion:

    This routine implements the recursion

      seed = 16807 * seed mod ( 2^31 - 1 )
      r8_uniform_01 = seed / ( 2^31 - 1 )

    The integer arithmetic never requires more than 32 bits,
    including a sign bit.

    If the initial seed is 12345, then the first three computations are

      Input     Output      R8_UNIFORM_01
      SEED      SEED

         12345   207482415  0.096616
     207482415  1790989824  0.833995
    1790989824  2035175616  0.947702

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    11 August 2004

  Author:

    John Burkardt

  Reference:

    Paul Bratley, Bennett Fox, Linus Schrage,
    A Guide to Simulation,
    Springer Verlag, pages 201-202, 1983.

    Pierre L'Ecuyer,
    Random Number Generation,
    in Handbook of Simulation
    edited by Jerry Banks,
    Wiley Interscience, page 95, 1998.

    Bennett Fox,
    Algorithm 647:
    Implementation and Relative Efficiency of Quasirandom
    Sequence Generators,
    ACM Transactions on Mathematical Software,
    Volume 12, Number 4, pages 362-376, 1986.

    Peter Lewis, Allen Goodman, James Miller,
    A Pseudo-Random Number Generator for the System/360,
    IBM Systems Journal,
    Volume 8, pages 136-143, 1969.

  Parameters:

    Input/output, int *SEED, the "seed" value.  Normally, this
    value should not be 0.  On output, SEED has been updated.

    Output, double R8_UNIFORM_01, a new pseudorandom variate, strictly between
    0 and 1.
    */
    double r8_uniform_01 (int *seed) {
      int k;
      double r;

      k = *seed / 127773;

      *seed = 16807 * (*seed - k * 127773) - k * 2836;

      if (*seed < 0)
        *seed = *seed + 2147483647;
      /*
        Although SEED can be represented exactly as a 32 bit integer,
        it generally cannot be represented exactly as a 32 bit real number!
      */
      r = (double) (*seed) * 4.656612875E-10;
      
      return r;
    }
  };
}

#endif
