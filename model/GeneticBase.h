#ifndef GENETIC_BASE_H
#define GENETIC_BASE_H

namespace openworld {
  template<class T>
    class GeneticData {
    double success;
    unsigned offspring;
    unsigned generation;
    T dna;

  public:
    GeneticData(T dna, unsigned generation, double success)
      : dna(dna) {
      this->success = success;
      this->offspring = 0;
      this->generation = generation;
    }

    GeneticData() {
    }

    void addOffspring(double success, unsigned count = 1) {
      cout << "Before AddOffspring: " << generation << ", " << this->success << endl;
      double selfweight = 10*exp(-(generation/50.0));
      this->success = (this->success * (offspring + selfweight) + success * count) / (offspring + selfweight + count);
      cout << "AddOffspring: " << selfweight << ", " << this->success << endl;
      offspring += count;
    }

    T& getDna() {
      return dna;
    }

    double getSuccess() {
      return success;
    }

    unsigned getOffspring() {
      return offspring;
    }

    unsigned getGeneration() {
      return generation;
    }

    // Serializable protocol

    friend istream& operator>>(istream& in, GeneticData<T>& sink) {
      return in >> sink.success >> sink.offspring >> sink.generation >> sink.dna;
    }

    friend ostream& operator<<(ostream& os, GeneticData<T>& source) {
      return os << source.success << " " << source.offspring << " " << source.generation << " " << source.dna;
    }
  };

  istream& operator>>(istream& in, vector<GeneticData<map<string, Measure> >* >& result) {
    unsigned count;
    in >> count;
    if (in.eof() || in.fail() || count == 0)
      return in; // nothing we can read

    for (unsigned ii = 0; ii < count; ii++) {      
      GeneticData<map<string, Measure> >* elt = new GeneticData<map<string, Measure> >();
      in >> *elt;
      result.push_back(elt);
    }

    return in;
  }

  ostream& operator<<(ostream& out, vector<GeneticData<map<string, Measure> >* >& source) {
    out << source.size() << " ";

    for (unsigned ii = 0; ii < source.size(); ii++)
      out << *source[ii];

    return out;      
  }

  istream& operator>>(istream& in, map<string, Measure>& result) {
    unsigned count;
    in >> count;

    for (unsigned ii = 0; ii < count; ii++) {
      Measure value = Measure::streamExtract(in);
      string name;
      in.ignore(); // ignore space
      getline(in, name);
      result.insert(pair<string, Measure>(name, value));
    }

    return in;
  }

  ostream& operator<<(ostream& out, map<string, Measure>& source) {
    out << source.size() << " ";

    for (map<string, Measure>::iterator it = source.begin() ; it != source.end(); it++)
      it->second.streamInsert(out) << it->first << endl;

    return out;
  }
}

#endif
