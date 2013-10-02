#include "Distribution.h"
#include "MonteCarloDistribution.h"
#include "GaussianDistribution.h"

namespace openworld {
  Distribution& operator-(double a, Distribution& b) {
    vector<double> samples;

    for (unsigned ii = 0; ii < 100; ii++)
      samples.push_back(a - b.random(rand()));

    return *tew_(MonteCarloDistribution(samples));
  }

  Distribution& Distribution::operator+(Distribution& b) {
    GaussianDistribution* ga = dynamic_cast<GaussianDistribution*>(this);
    GaussianDistribution* gb = dynamic_cast<GaussianDistribution*>(&b);
    if (ga && gb)
      return *tew_(GaussianDistribution(ga->getMean() + gb->getMean(), ga->getVariance() + gb->getVariance()));

    vector<double> samples;

    for (unsigned ii = 0; ii < 100; ii++)
      samples.push_back(this->random(rand()) + b.random(rand()));

    return *tew_(MonteCarloDistribution(samples));
  }

  Distribution& Distribution::operator*(Distribution& b) {
    vector<double> samples;

    for (unsigned ii = 0; ii < 100; ii++)
      samples.push_back(this->random(rand()) * b.random(rand()));

    return *tew_(MonteCarloDistribution(samples));
  }

  Distribution& Distribution::operator/(Distribution& b) {
    vector<double> samples;

    for (unsigned ii = 0; ii < 100; ii++)
      samples.push_back(this->random(rand()) / b.random(rand()));

    return *tew_(MonteCarloDistribution(samples));
  }
}
