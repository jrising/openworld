#include <multi/MultiModel.h>
#include <memory/Transients.h>
#include <sysdyn/NamedVariable.h>

using namespace openworld;

class MalthusPopulationCellModel : public SimpleTemporalModel {
 protected:
  Constant totalArea;
  Stock farmArea;
  Constant farmTimeConstant;

  Constant yield;
  Constant consume;

  Stock population;
  Constant naturalGrowth;
  Constant populationTimeConstant;

  NamedVariable grow;

 public:
 MalthusPopulationCellModel() :
  totalArea("Area", 1, Units::ha, *this),
    farmArea("FarmArea", .05, Units::ha, *this), // enough for 100 people
    farmTimeConstant("FarmTimeConstant", 1, Units::yr, *this),

    yield("Yield", 2.5, Units::mt / (Units::ha * Units::yr), *this),
    consume("Consumption", .25, Units::mt / (Units::individuals * Units::yr), *this),

    population("Population", 1, Units::individuals, *this),
    naturalGrowth("NaturalGrowth", 1.1, Units::none, *this),
    populationTimeConstant("PopulationTimeConstant", 3, Units::yr, *this),

    grow("Grow", Units::mt / Units::yr, *this) {

    TemporalVariable& demand = naturalGrowth * population * consume; // mt/yr
    TemporalVariable& farmAreaDesired = demand / yield; // ha
    TemporalVariable& farmAreaPossible = min(farmAreaDesired, totalArea);
    farmArea.setddt((farmAreaPossible - farmArea) / farmTimeConstant);
    grow = yield * farmArea; // mt/yr, = demand if enough area

    TemporalVariable& targetPopulation = (grow / consume);

    population.setddt((targetPopulation - population) / populationTimeConstant);
  }
};

