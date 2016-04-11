#include <multi/MultiModel.h>
#include <multi/DeltaDistribution.h>
#include <multi/GaussianDistribution.h>
#include <memory/Transients.h>

using namespace openworld;

class PopulationModel : public MultiModel {
 protected:
  DummyVariableHolder holder;
  Stock population;
  Constant growth;

 public:
  PopulationModel()
    : population("Population", 100, Units::individuals, holder), growth("Growth rate", .025, 1/Units::yr, holder) {

    population.setddt(growth * population);
  }

  virtual list<Variable> getVariables() {
    list<Variable> vars;
    vars.push_back(population);
    vars.push_back(growth);

    return vars;
  }

  virtual void setVariables(map<string, Distribution*>& vals) {
    population = vals[population.getName()]->getMode();
    growth = vals[growth.getName()]->getMode();
  }

  virtual map<string, Distribution*> evaluate(double tt) {
    map<string, Distribution*> result;
    double pop = population.evaluate(tt);
    result[population.getName()] = new GaussianDistribution(pop, (pop / 10) * (pop / 10)); //tew_(GaussianDistribution(pop, (pop / 10) * (pop / 10)));
    result[growth.getName()] = tew_(DeltaDistribution(growth.evaluate(tt)));

    return result;
  }
};

