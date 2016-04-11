#include <multi/MultiModel.h>
#include <memory/Transients.h>

using namespace openworld;

class FishingModel : public SimpleTemporalModel {
 protected:
  Stock biomass;
  Stock population;
  Constant effort;
  Constant capacity;
  Constant rate;
  Constant intrinsic;
  Constant phi;
  Constant theta;

 public:
 FishingModel() :
  biomass("Biomass", 1000, Units::mt, *this),
    population("Population", 100, Units::individuals, *this),
    effort("Effort", .005, 1/(Units::yr * Units::individuals), *this),
    capacity("Capacity", 1000, Units::mt, *this),
    rate("Rate", 1, 1/Units::yr, *this),
    intrinsic("Intrinsic", -1, 1/Units::yr, *this),
    phi("Phi", .1, Units::individuals / (Units::mt * Units::yr), *this),
    theta("Theta", .5, Units::individuals / Units::mt, *this) {

    TemporalVariable& extract = effort * population * biomass; // MT/yr
    biomass.increasesBy(rate * biomass * (1 - biomass / capacity) - extract);
    population.increasesBy(population * (intrinsic + phi * biomass / population + theta * extract / population));
  }
};

