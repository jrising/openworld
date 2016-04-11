#include <multi/MultiModel.h>
#include <memory/Transients.h>
#include <sysdyn/NamedVariable.h>

using namespace openworld;

class MalthusModel : public SimpleTemporalModel {
 protected:
  Constant totalArea; // Total area available for farm or forest (ha)
  Stock farmArea; // Area under cultivation (ha)
  Constant farmTimeConstant; // Rate at which area catches up to demand (yr)

  Constant yield; // Yield for agricultural land (MT / ha yr)
  Constant consume; // Demand for crops per person (MT / person yr)

  Constant hpue; // Forest harvest per unit effort (MT / person yr)
  Constant require; // Demand for forest harvest per person (NT / person yr)

  Stock population; // Population (persons)
  Constant naturalGrowth; // Natural growth of population per year (unitless)
  Constant populationTimeConstant; // Rate at which population adjusts to constraints (yr)

  Stock biomass; // Total forest biomass (MT)
  Constant capacityPerHa; // Carrying capacity of forests (MT / ha)
  Constant rate; // Logistic growth rate of forests (1 / yr)

  NamedVariable capacity; // Biomass at carrying capacity (MT)

 public:
 MalthusModel() :
  /// Define initial conditions and parameters

  totalArea("Area", 100, Units::ha, *this),
    farmArea("FarmArea", 5, Units::ha, *this), // enough for 100 people
    farmTimeConstant("FarmTimeConstant", 1, Units::yr, *this),

    yield("Yield", 2.5, Units::mt / (Units::ha * Units::yr), *this),
    consume("Consumption", .25, Units::mt / (Units::individuals * Units::yr), *this),

    hpue("GatherEfficiency", .05, Units::mt / (Units::individuals * Units::yr), *this),
    require("Requirement", .01, Units::mt / (Units::individuals * Units::yr), *this),

    population("Population", 100, Units::individuals, *this),
    naturalGrowth("NaturalGrowth", 1.1, Units::none, *this),
    populationTimeConstant("PopulationTimeConstant", 3, Units::yr, *this),
    
    biomass("Biomass", 95, Units::mt, *this),
    capacityPerHa("CapacityPerHa", 1, Units::mt / Units::ha, *this),
    rate("Rate", .1, 1/Units::yr, *this),

    capacity("Capacity", Units::mt, *this) {

    /// Farm demands and growth
    TemporalVariable& demand = naturalGrowth * population * consume; // demand including expected growth (mt/yr)
    TemporalVariable& farmAreaDesired = demand / yield; // farm area required to meet demand (ha)
    TemporalVariable& farmAreaPossible = min(farmAreaDesired, totalArea);
    farmArea.increasesBy((farmAreaPossible - farmArea) / farmTimeConstant);
    TemporalVariable& grow = yield * farmArea; // mt/yr, = demand if enough area

    // Forest gathering and growth
    TemporalVariable& natureArea = totalArea - farmArea; // Forest is whatever isn't farm
    capacity = capacityPerHa * natureArea; // Forest biomass at carrying capacity

    TemporalVariable& gatherPossible = hpue * population * biomass / capacity; // What can people try to gather? (mt/yr)
    TemporalVariable& gather = min(gatherPossible, naturalGrowth * population * require); // demand including expected growth (mt/yr)
    biomass.increasesBy(rate * biomass * (1 - biomass / capacity) - gather); // Logistic growth dynamics

    // Population constraints and growth
    TemporalVariable& targetPopulation = sqrt((grow / consume) * (gather / require)); // Geometric mean of constraints

    population.increasesBy((targetPopulation - population) / populationTimeConstant); // Exponential approach to it.
  }
};

