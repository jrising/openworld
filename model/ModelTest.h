#ifndef MODEL_TEST_H
#define MODEL_TEST_H

#include <limits>
#include <sys/statvfs.h>
#include "GeneticBase.h"
#include "SerializationTools.h"

namespace openworld {
  class ModelTest {
  public:
    virtual map<string, Measure> getParameters() = 0;
    virtual void setParameters(map<string, Measure>& params) = 0;

    virtual void prepare() = 0;
    virtual double evaluate() = 0;

    // Override this to provide a faster, noisy estimate of an evaluation
    virtual double partialEvaluate() {
      return evaluate();
    }

    // not all methods use suggestParameters
    virtual map<string, Measure> suggestParameters(map<string, Measure> params, unsigned count, double distance) {
      map<string, Measure> copy;

      for (map<string, Measure>::iterator it = params.begin(); it != params.end(); it++) {
        Measure attempt = it->second.randomAdjust(distance);
        cout << "Try " << it->first << " = " << attempt.getValue() << endl;
        copy.insert(pair<string, Measure>(it->first, attempt));
      }

      return copy;
    }

    virtual void calibrate(unsigned maxSteps, string saved = "") {
      calibrateByAsexualEvolution(maxSteps, saved);
    }

    // Continuously tries new values within the whole range allowed
    void calibrateByMonteCarlo(unsigned maxSteps) {
      prepare();
      map<string, Measure> params = getParameters();
      
      double minEnergy = numeric_limits<double>::infinity();
      map<string, Measure> bestParams;

      map<string, Measure> copy;
      for (unsigned step = 0; step < maxSteps; step++) {
        cout << "===== New attempt =====" << endl;

        // Select some parameters
        for (map<string, Measure>::iterator it = params.begin(); it != params.end(); it++) {
          Measure attempt = it->second.random();
          cout << "Try " << it->first << " = " << attempt.getValue() << endl;
          copy.insert(pair<string, Measure>(it->first, attempt));
        }

        setParameters(copy);
        double energy = evaluate();

        if (energy < minEnergy) {
          minEnergy = energy;
          bestParams = copy;
          cout << "Parameters improved!" << endl;
        }
      }
    }

    // Adjust current best values by small amounts
    void calibrateByRandomWalk(unsigned maxSteps) {
      prepare();

      map<string, Measure> bestParams = getParameters();

      cout << "===== Base Case =====" << endl;
      double minEnergy = evaluate();

      map<string, Measure> copy;
      for (unsigned step = 0; step < maxSteps; step++) {
        cout << "===== New attempt =====" << endl;

        // Select some parameters
        for (map<string, Measure>::iterator it = bestParams.begin(); it != bestParams.end(); it++) {
          Measure attempt = it->second.randomAdjust(sqrt(1 - step/(double) maxSteps));
          cout << "Try " << it->first << " = " << attempt.getValue() << endl;
          copy.insert(pair<string, Measure>(it->first, attempt));
        }

        setParameters(copy);
        double energy = evaluate();

        if (energy < minEnergy) {
          minEnergy = energy;
          bestParams = copy;
          cout << "Parameters improved!" << endl;
        }
      }
    }

    // Uses full evaluations
    // Doesn't keep track of offspring
    void calibrateByAsexualEvolutionFullEvaluation(unsigned maxSteps, string saved = "", unsigned organismCount = 10) {
      prepare();

      vector< GeneticData<map<string, Measure> >* > organisms;

      // Is there a saved file?
      ifstream isavefile;
      isavefile.open(saved.c_str());
      if (!isavefile.is_open()) {
        cout << "===== Base Case =====" << endl;
        map<string, Measure> params = getParameters();
        double success = partialEvaluate();

        GeneticData<map<string, Measure> >* eve = new GeneticData<map<string, Measure> >(params, 1, success);
        eve->addOffspring(success, 10);

        organisms.push_back(eve);
      } else {
        cout << "===== Loading Saved =====" << endl;

        isavefile >> organisms;
        isavefile.close();
      }

      for (unsigned step = 0; step < maxSteps; step++) {
        cout << "===== New attempt =====" << endl;
        
        // Choose a organism at random
        GeneticData<map<string, Measure> >* organism = organisms[rand() % organisms.size()];
        map<string, Measure>& params = organism->getDna();
        
        map<string, Measure> copy = suggestParameters(params, step, 1.0/organism->getGeneration());
        setParameters(copy);
        double success = evaluate();
        
        // Check if space
        struct statvfs buf;
        statvfs(saved.c_str(), &buf);
        float mbfree = (buf.f_bavail)*8.0/2048;

        cout << "Space left: " << mbfree << " MB" << endl;
        if (mbfree < 1024.0) {
          cout << "Low on space!  Exiting.";
          return;
        }

        if (organisms.size() < organismCount || success > organisms[0]->getSuccess()) {
          if (organisms.size() >= organismCount) {
            GeneticData< map<string, Measure> >* todelete = organisms[0];
            organisms.erase(organisms.begin());
            delete todelete;
          }

          unsigned ii;
          for (ii = 0; ii < organisms.size(); ii++) {
            if (organisms[ii]->getSuccess() > success)
              break;
          }
          organisms.insert(organisms.begin() + ii, new GeneticData<map<string, Measure> >(copy, organism->getGeneration() + 1, success));
          cout << "Organisms improved!" << endl;

          if (mbfree < 1.0) {
            cout << "Out of file space!  Exiting.";
            return;
          }

          ofstream osavefile;
          osavefile.open(saved.c_str());
          osavefile << organisms;
          osavefile.close();
        }
      }
    }

    // Uses partial evaluations, mutating past answers
    void calibrateByAsexualEvolution(unsigned maxSteps, string saved = "", unsigned organismCount = 10) {
      prepare();

      vector< GeneticData<map<string, Measure> >* > organisms;

      // Is there a saved file?
      ifstream isavefile;
      isavefile.open(saved.c_str());
      if (!isavefile.is_open()) {
        cout << "===== Base Case =====" << endl;
        map<string, Measure> params = getParameters();
        double success = partialEvaluate();

        GeneticData<map<string, Measure> >* eve = new GeneticData<map<string, Measure> >(params, 1, success);
        eve->addOffspring(success, 10);

        organisms.push_back(eve);
      } else {
        cout << "===== Loading Saved =====" << endl;

        isavefile >> organisms;
        isavefile.close();
      }

      for (unsigned step = 0; step < maxSteps; step++) {
        cout << "===== New attempt =====" << endl;
        
        // Choose a organism at random
        GeneticData<map<string, Measure> >* organism = organisms[rand() % organisms.size()];
        map<string, Measure>& params = organism->getDna();
        
        map<string, Measure> copy = suggestParameters(params, step, 1.0/organism->getGeneration());
        setParameters(copy);
        double success = partialEvaluate();
        
        // Include this in my offspring
        organism->addOffspring(success);

        // Check if space
        struct statvfs buf;
        statvfs(saved.c_str(), &buf);
        float mbfree = (buf.f_bavail)*8.0/2048;

        cout << "Space left: " << mbfree << " MB" << endl;
        if (mbfree < 1024.0) {
          cout << "Low on space!  Exiting.";
          return;
        }

        if (organisms.size() < organismCount || success > organisms[0]->getSuccess()) {
          if (organisms.size() >= organismCount) {
            GeneticData< map<string, Measure> >* todelete = organisms[0];
            organisms.erase(organisms.begin());
            delete todelete;
          }

          unsigned ii;
          for (ii = 0; ii < organisms.size(); ii++) {
            if (organisms[ii]->getSuccess() > success)
              break;
          }
          organisms.insert(organisms.begin() + ii, new GeneticData<map<string, Measure> >(copy, organism->getGeneration() + 1, success));
          cout << "Organisms improved!" << endl;

          if (mbfree < 1.0) {
            cout << "Out of file space!  Exiting.";
            return;
          }

          ofstream osavefile;
          osavefile.open(saved.c_str());
          osavefile << organisms;
          osavefile.close();
        }
      }
    }

    void testSaving(string filename, unsigned count) {
      vector< GeneticData<map<string, Measure> >* > tosave, compare;

      map<string, Measure> params = getParameters();
      for (unsigned ii = 0; ii < count; ii++) {
        map<string, Measure> copy = suggestParameters(params, ii, 1.0);
        tosave.push_back(new GeneticData<map<string, Measure> >(copy, rand() % 100, .01 * (rand() % 100)));
      }

      ofstream osavefile;
      osavefile.open(filename.c_str());
      osavefile << tosave;
      osavefile.close();

      ifstream isavefile;
      isavefile.open(filename.c_str());
      if (isavefile.is_open()) {
        isavefile >> compare;
        isavefile.close();

        // compare
        if (compare.size() != tosave.size())
          cout << "Size differs" << endl;

        for (unsigned ii = 0; ii < compare.size(); ii++) {
          if (compare[ii]->getSuccess() != tosave[ii]->getSuccess() ||
              compare[ii]->getGeneration() != tosave[ii]->getGeneration())
            cout << "GeneticBase properties differ" << endl;
          
          map<string, Measure>& compareParams = compare[ii]->getDna();
          map<string, Measure>& tosaveParams = tosave[ii]->getDna();
          if (compareParams.size() != tosaveParams.size())
            cout << "DNA Size differs" << endl;

          map<string, Measure>::iterator tosaveParamsIt, compareParamsIt;
          for (tosaveParamsIt = tosaveParams.begin(); tosaveParamsIt != tosaveParams.end(); tosaveParamsIt++) {
            compareParamsIt = compareParams.find(tosaveParamsIt->first);
            if (compareParamsIt == compareParams.end()) {
              cout << "GeneticBase DNA incomplete: Missing " << tosaveParamsIt->first << endl;
              continue;
            }
            if (compareParamsIt->first.compare(tosaveParamsIt->first) ||
                compareParamsIt->second != tosaveParamsIt->second)
              cout << "GeneticBase DNA differs" << endl;
          }
        }
      } else
        cout << "File missing" << endl;
      
      cout << "Test complete" << endl;
    }
    
  /*void calibrateByGradient(unsigned maxSteps, double minDiff = double.NaN) {
    double initial = evaluate();
    if (isnan(minDiff))
      minDiff = initial * 1e-6

    // Try adjusting each parameter
    map<string, Measure> params = model.getParameters();
    map<string, Measure> grads;
    for (map<string, Measure>::iterator it = params.first(); it != params.end(); it++) {
      // New parameters with one change
      map<string, Measure> copy = params.copy();
      Measure adjust = it->second * 1e-6;
      copy[it->first] = it->second + adjust;
      model.setParameters(copy);
      
      grads[it->first] = (evaluate() - initial) / adjust;
    }

    // Calculate gradient
    map<string, Measure> gradient;
    for (map<string, Measure>::iterator it = params.first(); it != params.end(); it++) {
      
    }
    }*/
  };
}

#endif
