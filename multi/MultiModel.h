#ifndef MULTI_MODEL_H
#define MULTI_MODEL_H

#include <multi/SimpleModel.h>
#include <multi/MonteCarloDistribution.h>
#include <multi/GaussianDistribution.h>
#include <multi/DeltaDistribution.h>

namespace openworld {
  class MultiModel {
  public:
    virtual list<Variable> getVariables() = 0;
    virtual void setVariables(map<string, Distribution*>& vars) = 0;
    virtual map<string, Distribution*> evaluate(double tt) = 0;
  };

  template<class T>
  class MonteCarloModel : public MultiModel {
  protected:
    vector<SimpleModel*> runs;

  public:
    MonteCarloModel(unsigned runs) {
      for (unsigned ii = 0; ii < runs; ii++)
        this->runs.push_back((SimpleModel*) new T());
    }

    virtual list<Variable> getVariables() {
      return runs[0]->getVariables();
    }

    virtual void setVariables(map<string, Distribution*>& vars) {
      for (unsigned ii = 0; ii < runs.size(); ii++) {
        map<string, double> dvars;
        for (map<string, Distribution*>::iterator it = vars.begin(); it != vars.end(); it++)
          dvars[it->first] = it->second->random(ii + 1);

        runs[ii]->setVariables(dvars);
      }
    }

    virtual map<string, Distribution*> evaluate(double tt) {
      map<string, Distribution*> vars;

      for (unsigned ii = 0; ii < runs.size(); ii++) {
        map<string, double> dvars = runs[ii]->evaluateVariables(tt);
        if (vars.size() == 0) {
          for (map<string, double>::iterator it = dvars.begin(); it != dvars.end(); it++)
            vars[it->first] = new MonteCarloDistribution(vector<double>());
        }

        for (map<string, double>::iterator it = dvars.begin(); it != dvars.end(); it++)
          ((MonteCarloDistribution*) vars[it->first])->addSample(it->second);
      }

      return vars;
    }
  };

  class GaussianNoiseModel : public MultiModel, public IDeletable {
  protected:
    SimpleModel* model; // owns
    double fraction;

  public:
    GaussianNoiseModel(SimpleModel* model, double fraction) {
      this->model = model;
      this->fraction = fraction;
    }

    virtual ~GaussianNoiseModel() {
      delete model;
    }

    // don't report model, because I own it.
    //virtual list<IDeletable*> getContainedPointers() const {}

    virtual list<Variable> getVariables() {
      return model->getVariables();
    }

    virtual void setVariables(map<string, Distribution*>& vars) {
      map<string, double> dvars;
      for (map<string, Distribution*>::iterator it = vars.begin(); it != vars.end(); it++)
        dvars[it->first] = it->second->getMode();

      model->setVariables(dvars);
    }

    virtual map<string, Distribution*> evaluate(double tt) {
      map<string, Distribution*> vars;

      map<string, double> dvars = model->evaluateVariables(tt);
      for (map<string, double>::iterator it = dvars.begin(); it != dvars.end(); it++)
        vars[it->first] = tew_(GaussianDistribution(it->second, (fraction * it->second) * (fraction * it->second)));

      return vars;
    }
  };

  class DeltaModel : public MultiModel, public IDeletable {
  protected:
    SimpleModel* model; // owns

  public:
    DeltaModel(SimpleModel* model) {
      this->model = model;
    }

    virtual ~DeltaModel() {
      delete model;
    }

    // don't report model, because I own it.
    //virtual list<IDeletable*> getContainedPointers() const {}

    virtual list<Variable> getVariables() {
      return model->getVariables();
    }

    virtual void setVariables(map<string, Distribution*>& vars) {
      map<string, double> dvars;
      for (map<string, Distribution*>::iterator it = vars.begin(); it != vars.end(); it++)
        dvars[it->first] = it->second->getMode();

      model->setVariables(dvars);
    }

    virtual map<string, Distribution*> evaluate(double tt) {
      map<string, Distribution*> vars;

      map<string, double> dvars = model->evaluateVariables(tt);
      for (map<string, double>::iterator it = dvars.begin(); it != dvars.end(); it++)
        vars[it->first] = tew_(DeltaDistribution(it->second));

      return vars;
    }
  };
}

#endif
