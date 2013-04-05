/******************************************************************************\
 * Variable: Any kind of value which contructs before it runs
 ******************************************************************************
 * 
\******************************************************************************/
#ifndef VARIABLE_H
#define VARIABLE_H

namespace openworld {
  class Variable {
  protected:
    string name;
    Dimensions dims;
	
  public:
    Variable(string name, Dimensions dims) {
      this.name = name;
      this.dims = dims;
    }
		
    string ToString() {
      return string.Format ("{0} [{1}]", Name, Dimensions);
    }
		
    string getName() {
      return name;
    }
		
    Dimensions getDimensions() {
      return dims;
    }
				
    // Mathematics
		
    static Variable operator-(Variable a) {
      return Variable("-" + a.name, a.Dimensions);
    }
		
    Variable operator+(Variable b) {
      if (dims != b.dims)
        throw new ArgumentException("Dimensions mismatch to +");
      return Variable("(" + a.name + " + " + b.name + ")", dims);
    }
		
    Variable operator-(Variable b) {
      if (dims != b.dims)
        throw new ArgumentException("Dimensions mismatch to -");
      return Variable("(" + a.name + " - " + b.name + ")", dims);
    }
		
    Variable operator*(Variable b) {
      return Variable(a.name + " " + b.name, dims * b.dims);
    }

    Variable operator/(Variable b) {
      return Variable("(" + a.name + ")/(" + b.name + ")", dims / b.dims);
    }
  };
}

#endif
