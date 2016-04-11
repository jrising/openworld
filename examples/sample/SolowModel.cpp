void main() {
  Stock labor = new SimplePopulationStock("Labor (L)", 10000, .1);
  Stock capital = new Stock("Capital (K)", 1000, GlobalDimensions.get("k"));
  Stock technology = new Stock("Technology (A)", 4, GlobalDimensions.get("a"));
  RandomVariable shock = new RandomVariable("Shock (epsilon)", .9, 1.1, Dimensionless.Instance);

  //technology.IncreasesBy(new Constant("Technological Advancement (gamma)", .1, GlobalDimensions.get("a").DividedBy(GlobalDimensions.Time).DividedBy(GlobalDimensions.get("k"))) * new PassiveDelay(capital, 0) / new PassiveDelay(labor, 0));

  TemporalVariable effectiveLabor = labor * technology;
  TemporalVariable production = new Function("Production (Y)", x => Math.Pow(x[0], x[2]) * Math.Pow(x[1], (1 - x[2])) * x[3],
                                             GlobalDimensions.get("k"), capital, effectiveLabor, new Constant("Preference (alpha)", .3, Dimensionless.Instance), shock);
  Constant savingsRate = new Constant("Savings Rate (s)", .1, GlobalDimensions.Time.RaisedTo(-1));
  capital.IncreasesBy(production * savingsRate
                      - (capital * (new Constant("Depreciation Rate (delta)", .1, GlobalDimensions.Time.RaisedTo(-1)))));
			
  TemporalVariable capitalPerEffectiveLaborer = capital / effectiveLabor;
  TemporalVariable productionPerLaborer = production / labor;

  double[,] result = new double[100, 3];
  for (int tt = 0; tt < 100; tt++) {
    result[tt, 0] = capitalPerEffectiveLaborer.Evaluate(tt);
    result[tt, 1] = (1 - savingsRate.Evaluate(tt)) * productionPerLaborer.Evaluate(tt);
    result[tt, 2] = labor.Evaluate(tt);
  }
			
  return result;
}
