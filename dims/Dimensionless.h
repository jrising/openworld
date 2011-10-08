#ifndef DIMENSIONS_H
#define DIMENSIONS_H

using openworld;

class Dimensionless : public Dimensions {
 private:
  static Dimensionless instance;
		
  Dimensionless() { }

 public:
  static Dimensionless instance() {
    if (instance == null)
      instance = new Dimensionless();
    return instance;
  }
		
  Dimensions raisedTo(double power) {
    return this;
  }
		
  Dimensions times(IDimensions other) {
    return other;
  }
		
  Dimensions dividedBy(IDimensions other) {
    return other.RaisedTo(-1);
  }
  
  bool equals(IDimensions other) {
    return this == other;
  }
  
  KeyValuePair<IDimensions, double>[] factors() {
    return null;
  }
  
  string toString() {
    return "";
  }
}

#endif
