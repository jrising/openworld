#include <multi/MultiModel.h>
#include <sysdyn/NamedVariable.h>
#include <memory/Transients.h>

using namespace openworld;

class MalthusForestCellModel : public SimpleTemporalModel {
 protected:
  //Stock population;
  //Constant growthRate;

  Constant population;

  Constant overHarvest;
  Constant hpue;
  Constant require;

  Stock biomass;
  Constant capacity;
  Constant rate;

  NamedVariable gather;

 public:
 MalthusForestCellModel() :
  population("Population", 1, Units::individuals, *this),

    overHarvest("OverHarvest", 1.1, Units::none, *this),
    hpue("GatherEfficiency", .05, Units::mt / (Units::individuals * Units::yr), *this),
    require("Requirement", 0.0125, Units::mt / (Units::individuals * Units::yr), *this), // should be .01, but miscalibrated

    biomass("Biomass", 1, Units::mt, *this),
    capacity("Capacity", 1, Units::mt, *this),
    rate("Rate", .1, 1/Units::yr, *this), // should be .1, but miscalibrated

    gather("Gather", Units::mt / Units::yr, *this) {

    TemporalVariable& gatherPossible = hpue * population * biomass / capacity; // mt/yr
    gather = min(gatherPossible, overHarvest * population * require);

    biomass.setddt(rate * biomass * (1 - biomass / capacity) - gather);
  }

  double getPopulation() {
    return population.getDouble();
  }

  void setPopulation(double pop) {
    population = pop;
  }

  double getBiomass() {
    return biomass.getLevel();
  }

  void setBiomass(double bio) {
    biomass = bio;
  }

  double getGather() {
    return gather.getDouble();
  }

  double getRequire() {
    return require.getDouble();
  }
};

