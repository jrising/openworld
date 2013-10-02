#ifndef MONTE_CARLO_DISTRIBUTION_H
#define MONTE_CARLO_DISTRIBUTION_H

#include "Distribution.h"
#include <vector>
#include <stdexcept>
#include "../memory/Transients.h"
#include <math.h>

namespace openworld {
  class MonteCarloDistribution : public Distribution {
  protected:
    vector<double> samples;
    unsigned preferredSize;
    
  public:
    MonteCarloDistribution(vector<double> samples) {
      this->samples = samples;
      preferredSize = samples.size();
    }

    void addSample(double sample) {
      samples.push_back(sample);
      preferredSize++;
    }

    virtual double getMode() {
      // calculate mean (mode for triangular kernel?)
      double sum = 0;
      for (unsigned ii = 0; ii < samples.size(); ii++)
        sum += samples[ii];
      return sum / samples.size();
    }

    virtual double getMaxy() {
      double mymax = 0;
      for (unsigned ii = 0; ii < samples.size(); ii++)
        mymax = max(samples[ii], mymax);

      return mymax;
    }

    void coutall() {
      for (unsigned ii = 0; ii < samples.size(); ii++)
        cout << samples[ii] << endl;
    }

    bool anyNaN() {
      for (unsigned ii = 0; ii < samples.size(); ii++)
        if (isnan(samples[ii]))
          return true;

      return false;
    }
    
    virtual Distribution& merge(Distribution& other, bool flippable = true) {
      MonteCarloDistribution* omc = dynamic_cast<MonteCarloDistribution*>(&other);
      if (omc != NULL) {
        vector<double> comboSamples(samples);
        for (unsigned ii = 0; ii < omc->samples.size(); ii++)
          comboSamples.push_back(omc->samples[ii]);
        
        return *tew_(MonteCarloDistribution(comboSamples));
      }

      ContinuousDistribution* oc = dynamic_cast<ContinuousDistribution*>(&other);
      if (oc != NULL) {
        unsigned seed = rand();
        // combine both
        vector<double> mergeSamples(samples);
        for (unsigned ii = 0; ii < samples.size(); ii++)
          mergeSamples.push_back(oc->random(seed));

        return *tew_(MonteCarloDistribution(mergeSamples));
      }

      if (flippable)
        return other.merge(*this, false);

      throw runtime_error("Cannot merge with given distribution");
    }

    virtual void mergePurge() {
        // purge samples
        while (samples.size() > preferredSize)
          samples.erase(samples.begin() + (rand() % samples.size()));
    }

    virtual double random(unsigned seed) {
      srand(seed);
      return samples[rand() % samples.size()];
    }
  };
}

#endif
