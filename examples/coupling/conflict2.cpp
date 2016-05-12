#include <iostream>
#include <sysdyn/Stock.h>
#include <sysdyn/TemporalVariable.h>
#include <sysdyn/Constant.h>
#include <measure/Units.h>
#include <memory/Transients.h>

// For multimodel
#include <multi/Distribution.h>
#include <multi/BayesianMerge.h>
#include "MalthusModel.h"
#include "MalthusForestCellModel.h"
#include "MalthusPopulationCellModel2.h"

using namespace openworld;

void disperseBiomass(vector<MalthusPopulationCellModel2*>& pops, vector<MalthusForestCellModel*>& fors, double dt, double rate);
void distributeGathering(vector<MalthusPopulationCellModel2*>& pops, vector<MalthusForestCellModel*>& fors, double dt, double rate);

int main(int argc, const char* argv[])
{
  Stock::deltat = .01;
  unsigned count = 100;
  bool showagg = true, mergefull = true;

  if (showagg)
    cout << "model,time,population,biomass,farm" << endl;

  try {
    MalthusModel malmod;
    vector<MalthusPopulationCellModel2*> pops;
    vector<MalthusForestCellModel*> fors;

    for (unsigned ii = 0; ii < 20; ii++) {
      map<string, double> setpops;
      setpops["Population"] = (20 - ii) * .2857 + 2; // so sum is 100
      setpops["FarmArea"] = ((20 - ii) * .2857 + 2) / 10;

      MalthusPopulationCellModel2* mod = new MalthusPopulationCellModel2();
      mod->setVariables(setpops);
      pops.push_back(mod);
    }
    for (unsigned ii = 20; ii < count; ii++) {
      map<string, double> setpops;
      setpops["Population"] = 0;
      setpops["FarmArea"] = 0;

      MalthusPopulationCellModel2* mod = new MalthusPopulationCellModel2();
      mod->setVariables(setpops);
      pops.push_back(mod);
    }

    for (unsigned ii = 0; ii < 20; ii++) {
      map<string, double> setfors;
      setfors["Biomass"] = 0;

      MalthusForestCellModel* mod = new MalthusForestCellModel();
      mod->setVariables(setfors);
      fors.push_back(mod);
    }
    for (unsigned ii = 20; ii < count; ii++) {
      MalthusForestCellModel* mod = new MalthusForestCellModel();
      fors.push_back(mod);
    }

    distributeGathering(pops, fors, .1, 3);

    for (double tt = 0; tt < 100; tt += .1) {
      map<string, double> malvars = malmod.evaluateVariables(tt);

      GaussianDistribution malpop(malvars["Population"], .01*malvars["Population"]*malvars["Population"]);
      GaussianDistribution malbio(malvars["Biomass"], .01*malvars["Biomass"]*malvars["Biomass"]);
      GaussianDistribution malfarm(malvars["FarmArea"], .01*malvars["FarmArea"]*malvars["FarmArea"]);

      double totalPopulation = 0;
      double totalBiomass = 0;
      double totalFarmArea = 0;

      if (mergefull) {
        vector<GaussianDistribution*> lowpops;
        vector<GaussianDistribution*> lowbios;
        vector<GaussianDistribution*> lowfarms;
        for (unsigned ii = 0; ii < count; ii++) {
          map<string, double> vars = pops[ii]->evaluateVariables(tt);
          if (vars["Population"] > 0) {
            totalFarmArea += vars["FarmArea"];
            totalPopulation += vars["Population"];
            lowpops.push_back(new GaussianDistribution(vars["Population"], .25*vars["Population"]*vars["Population"])); // or .1
            lowfarms.push_back(new GaussianDistribution(vars["FarmArea"], .25*vars["FarmArea"]*vars["FarmArea"]));
          }
        }
        for (unsigned ii = 0; ii < count; ii++) {
          if (pops[ii]->getPopulation() == 0) {
            map<string, double> vars = fors[ii]->evaluateVariables(tt);
            totalBiomass += vars["Biomass"];
            lowbios.push_back(new GaussianDistribution(vars["Biomass"], .25*vars["Biomass"]*vars["Biomass"]));
          }
        }

        BayesianMerge::mergeGaussianHierarchy(lowpops, &malpop);
        BayesianMerge::mergeGaussianHierarchy(lowbios, &malbio);
        BayesianMerge::mergeGaussianHierarchy(lowfarms, &malfarm);

        for (unsigned ii = 0; ii < count; ii++) {
          if (pops[ii]->getPopulation() > 0) {
            pops[ii]->setPopulation(lowpops[ii]->getMean());
            pops[ii]->setFarmArea(lowfarms[ii]->getMean());
          }
        }
        unsigned jj = 0;
        for (unsigned ii = 0; ii < count; ii++) {
          if (pops[ii]->getPopulation() == 0)
            fors[ii]->setBiomass(lowbios[jj++]->getMean());
        }
        /*map<string, double> merged;
        merged["Population"] = malpop.getMean();
        merged["Biomass"] = malbio.getMean();
        merged["FarmArea"] = malfarm.getMean();
        malmod.setVariables(merged);*/
      } else {
        for (unsigned ii = 0; ii < count; ii++) {
          map<string, double> vars = pops[ii]->evaluateVariables(tt);
          totalFarmArea += vars["FarmArea"];
          totalPopulation += vars["Population"];
        }
        for (unsigned ii = 0; ii < count; ii++) {
          if (pops[ii]->getPopulation() == 0) {
            map<string, double> vars = fors[ii]->evaluateVariables(tt);
            totalBiomass += vars["Biomass"];
          }
        }
      }

      // Expand as needed
      unsigned fulls = 0;
      for (unsigned ii = 0; ii < count; ii++) {
        if (pops[ii]->isFull())
          fulls++;
      }

      if (fulls > 0) {
        unsigned news = 0;
        for (unsigned ii = 0; ii < count; ii++) {
          if (pops[ii]->getPopulation() == 0) {
            pops[ii]->setPopulation((10 / 1.1) - 5);
            pops[ii]->setFarmArea(.5);
            fors[ii]->setBiomass(0);
            news++;
            
            if (--fulls == 0)
              break;
          }
        }

        if (news > 0) {
          for (unsigned ii = 0; ii < count; ii++) {
            if (pops[ii]->isFull()) {
              pops[ii]->setPopulation(pops[ii]->getPopulation() - ((10 / 1.1) - 5));
              if (--news == 0)
                break;
            }
          }
        }
      }

      distributeGathering(pops, fors, .1, 3);
      disperseBiomass(pops, fors, .1, .1);

      if (showagg) {
        if (mergefull)
          cout << "10," << tt << "," << totalPopulation << "," << totalBiomass << "," << totalFarmArea << endl;
        else
          cout << "9," << tt << "," << totalPopulation << "," << totalBiomass << "," << totalFarmArea << endl;          
      } else {
        cout << "pop,";
        for (unsigned ii = 0; ii < count; ii++) {
          cout << pops[ii]->getPopulation();
          if (ii < count - 1)
            cout << ",";
        }
        cout << endl;
        cout << "for,";
        for (unsigned ii = 0; ii < count; ii++) {
          cout << fors[ii]->getBiomass();
          if (ii < count - 1)
            cout << ",";
        }
        cout << endl;
      }
    }

    /* // multilevel merge
      MalthusModel malmod;
    vector<MalthusForestCellModel> formods;
    vector<MalthusPopulationCellModel2> popmods;
    
    unsigned ii = 0;
    for (double tt = 0; tt < 100; tt += .1) {
      map<string, double> vars = malmod.evaluateVariables(tt);
      // Merge all models
      map<string, double> summed;
      for (int ii = 0; ii < count; ii++) {
        map<string, double> forvars = formods[ii].evaluateVariables(tt);
        for (map<string, double>::iterator it = forvars.begin(); it != forvars.end(); it++) {
          if (merged.find(it->first) == merged.end())
            merged[it->first] = it->second;
          else
            merged[it->first] = merged[it->first] + it->second;
        }
        merged.erase("Population");

        map<string, double> popvars = popmods[ii].evaluateVariables(tt);
        for (map<string, double>::iterator it = popvars.begin(); it != popvars.end(); it++) {
          if (merged.find(it->first) == merged.end())
            merged[it->first] = it->second;
          else
            merged[it->first] = merged[it->first] + it->second;
        }
      }
      }*/
  } catch (exception& e) {
    cout << e.what() << endl;
    throw e;
  }
}

void distributeGathering(vector<MalthusPopulationCellModel2*>& pops, vector<MalthusForestCellModel*>& fors, double dt, double rate) {
  double totalpop = 0;
  double todistribute = 0;
  double forestsfor = 0;
  double forestsgathered = 0;
  // distribute the foragers
  unsigned populated = 0;
  for (unsigned ii = 0; ii < pops.size(); ii++) {
    totalpop += pops[ii]->getPopulation();
    if (pops[ii]->getPopulation() > 0)
      populated++;
    todistribute += pops[ii]->getPopulation();
  }
  for (unsigned ii = 0; ii < fors.size(); ii++) {
    if (fors[ii]->getBiomass() > 0 && pops[ii]->getPopulation() == 0) {
      forestsfor += fors[ii]->getPopulation();
      forestsgathered += fors[ii]->getGather();
      double foragers = min(todistribute, 1 + todistribute / (pops.size() - ii));
      fors[ii]->setPopulation(foragers);
      todistribute -= foragers;
    }
  }

  // distribute the forest products back
  double factor = (forestsgathered / fors[0]->getRequire()) / forestsfor;
  for (int ii = pops.size() - 1; ii >= 0; ii--) {
    if (pops[ii]->getPopulation() > 0) {
      double newfactor = pops[ii]->getTargetPopulationFactor() + (factor - pops[ii]->getTargetPopulationFactor()) * dt * rate;
      pops[ii]->setTargetPopulationFactor(newfactor);
      factor = pops[ii]->getTargetPopulationFactor();
    }
  }
}

void disperseBiomass(vector<MalthusPopulationCellModel2*>& pops, vector<MalthusForestCellModel*>& fors, double dt, double rate) {
  vector<double> leaves;
  for (unsigned ii = 0; ii < fors.size(); ii++)
    leaves.push_back(fors[ii]->getBiomass() * rate * dt);

  for (unsigned ii = 1; ii < fors.size(); ii++) {
    if (pops[ii]->getPopulation() == 0) {
      if (ii < fors.size() - 1)
        fors[ii]->setBiomass(fors[ii]->getBiomass() - leaves[ii] + leaves[ii-1] / 2 + leaves[ii+1] / 2);
      else
        fors[ii]->setBiomass(fors[ii]->getBiomass() - leaves[ii] / 2 + leaves[ii-1] / 2);
    }
  }
}
