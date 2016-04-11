#include <multi/MultiModel.h>
#include <memory/Transients.h>
#include <sysdyn/NamedVariable.h>

using namespace openworld;

class MalthusPopulationCellModel2 : public SimpleTemporalModel {
 protected:
  Constant totalArea;
  Stock farmArea;
  Constant farmTimeConstant;

  Constant yield;
  Constant consume;

  Stock population;
  Constant naturalGrowth;
  Constant populationTimeConstant;

  Constant targetPopulationFactor;
  NamedVariable grow;
  NamedVariable farmAreaDesired;
  NamedVariable targetPopulation;

 public:
 MalthusPopulationCellModel2() :
  totalArea("Area", 1, Units::ha, *this),
    farmArea("FarmArea", .05, Units::ha, *this), // enough for 100 people
    farmTimeConstant("FarmTimeConstant", 1, Units::yr, *this),

    yield("Yield", 2.5, Units::mt / (Units::ha * Units::yr), *this),
    consume("Consumption", .25, Units::mt / (Units::individuals * Units::yr), *this),

    population("Population", 1, Units::individuals, *this),
    naturalGrowth("NaturalGrowth", 1.1, Units::none, *this),
    populationTimeConstant("PopulationTimeConstant", 3, Units::yr, *this),

    targetPopulationFactor("TargetPopulationFactor", 1.1, Units::none, *this),
    grow("Grow", Units::mt / Units::yr, *this),
    farmAreaDesired("FarmAreaDesired", Units::ha, *this),
    targetPopulation("TargetPopulation", Units::individuals, *this) {
    
    TemporalVariable& demand = naturalGrowth * population * consume; // mt/yr
    farmAreaDesired = demand / yield; // ha
    TemporalVariable& farmAreaPossible = min(farmAreaDesired, totalArea);
    farmArea.increasesBy((farmAreaPossible - farmArea) / farmTimeConstant);
    grow = yield * farmArea; // mt/yr, = demand if enough area

    targetPopulation = sqrt((grow / consume) * targetPopulationFactor * population);

    population.increasesBy((targetPopulation - population) / populationTimeConstant);
  }

  double getPopulation() {
    return population.getLevel();
  }

  double getGrow() {
    return grow.getDouble();
  }

  void setPopulation(double pop) {
    population = pop;
  }

  double getFarmArea() {
    return farmArea.getLevel();
  }

  void setFarmArea(double area) {
    farmArea = area;
  }

  double getTargetPopulationFactor() {
    return targetPopulationFactor.getDouble();
  }

  void setTargetPopulationFactor(double value) {
    targetPopulationFactor = value;
  }

  bool isFull() {
    return farmAreaDesired.getDouble() > totalArea.getDouble();
  }
};

