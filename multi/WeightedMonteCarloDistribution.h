#ifndef WEIGHTED_MONTE_CARLO_DISTRIBUTION_H
#define WEIGHTED_MONTE_CARLO_DISTRIBUTION_H

#include "Distribution.h"
#include <vector>
#include "../memory/Transients.h"

namespace openworld {
  class WeightedMonteCarloDistribution : public Distribution {
  protected:
    vector<double> samples;
    vector<double> weights;
    double weightsum;
    
  public:
    WeightedMonteCarloDistribution(vector<double> samples, vector<double> weights = vector<double>()) {
      this->samples = samples;
      if (weights.size() == samples.size())
        this->weights = weights;
      else if (weights.size() == 0)
        for (unsigned ii = 0; ii < samples.size(); ii++)
          weights.push_back(1);
      else
        throw runtime_error("Unknown weights vector in Monte Carlo");

      weightsum = 0;
      for (unsigned ii = 0; ii < samples.size(); ii++)
        weightsum += weights[ii];
    }

    void addSample(double sample, double weight = 1) {
      samples.push_back(sample);
      weights.push_back(weight);
      weightsum += weight;
    }

    virtual double getMode() {
      // calculate mean (mode for triangular kernel?)
      double sum = 0;
      for (unsigned ii = 0; ii < samples.size(); ii++)
        sum += samples[ii] * weights[ii];
      return sum / weightsum;
    }
    
    virtual Distribution& merge(Distribution& other, bool flippable = true) {
      WeightedMonteCarloDistribution* omc = dynamic_cast<WeightedMonteCarloDistribution*>(&other);
      if (omc != NULL) {
        vector<double> comboSamples(samples);
        vector<double> comboWeights(weights);
        for (unsigned ii = 0; ii < omc->samples.size(); ii++) {
          comboSamples.push_back(omc->samples[ii]);
          // first scaling ensures that all is on equal footing, second scales by number of samples
          comboWeights.push_back(omc->weights[ii] * (weightsum / omc->weightsum) * (omc->samples.size() / (double) samples.size()));
        }
        
        return *tew_(WeightedMonteCarloDistribution(comboSamples, comboWeights));
      }

      ContinuousDistribution* oc = dynamic_cast<ContinuousDistribution*>(&other);
      if (oc != NULL) {
        vector<double> mergeSamples(samples);
        vector<double> mergeWeights;
        for (unsigned ii = 0; ii < samples.size(); ii++)
          mergeWeights.push_back(weights[ii] * oc->pdf(samples[ii]));

        return *tew_(WeightedMonteCarloDistribution(mergeSamples, mergeWeights));
      }

      if (flippable)
        return other.merge(*this, false);

      throw runtime_error("Cannot merge with given distribution");
    }

    virtual double random(unsigned seed) {
      srand(seed);
      double choice = weightsum * ((double)rand() / (double)RAND_MAX);
      for (unsigned ii = 0; ii < samples.size(); ii++) {
        if (weights[ii] > choice)
          return samples[ii];
        choice -= weights[ii];
      }

      throw runtime_error("random returned choice outside of bounds");
    }
  };
}

#endif
