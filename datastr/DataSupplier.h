#ifndef DATA_SUPPLIER_H
#define DATA_SUPPLIER_H

#include "../ext/gzstream/gzstream.h"

namespace openworld {
  template<class T>
  class DataSupplier {
  public:
    virtual ~DataSupplier() {}

    virtual T get() = 0;
    virtual int length() = 0; // -1 for undetermined
    virtual bool done() = 0;
  };

  template<class T>
  class DelimitedSupplier : public DataSupplier<T> {
  protected:
    istream* file;
    istringstream* linestream;

    T (*parser)(string);
    char delimiter;

    bool allowblank;

    unsigned rows;
    unsigned cols;
    unsigned rowcols; // cols of the current row
    
  public:
    DelimitedSupplier(string filepath, T (*parser)(string) = NULL, char delimiter = ',', bool allowblank = true) {
      if (filepath.compare(filepath.length() - 3, 3, ".gz") == 0)
        file = new igzstream(filepath.c_str());
      else
        file = new ifstream(filepath.c_str());

      if (!file->good())
        throw runtime_error("Cannot open file");

      if (!parser)
        parser = FileParser<T>::parseSimple;
      this->parser = parser;
      this->delimiter = delimiter;
      this->allowblank = allowblank;

      rows = cols = rowcols = 0;
      linestream = NULL;
    }

    virtual ~DelimitedSupplier() {
      delete file;
    }

    virtual T get() {
      if (!linestream || !linestream->good()) {
        if (linestream) {
          delete linestream;
          linestream = NULL; // in case that's the end

          if (cols == 0)
            cols = rowcols;
          else if (cols != rowcols) {
            if (!file->good())
              return T();

            stringstream errstr;
            errstr << "Inconsistent row lengths: " << cols << " <> " << rowcols << " at " << rows;
            throw runtime_error(errstr.str());
          }
        }

        string line = "\n";
        while (file->good() && (line[0] == '\n' || (line[0] == '#' && delimiter != '#')))
          getline(*file, line);
        if (!file->good()) {
          cout << "File done!" << endl;
          return T();
        }

        linestream = new istringstream(line);
        rowcols = 0;
      }

      string item;
      getline(*linestream, item, delimiter);
      if (!allowblank && item.size() == 0) {
        if (!linestream->good())
          rows++;
       
        return get();
      }

      rowcols++;

      if (!allowblank && linestream->tellg() == (int) linestream->str().size())
        linestream->ignore(1); // move to where linestream is not good

      if (!linestream->good())
        rows++;

      return parser(item);
    }

    virtual int length() {
      if (!done())
        return -1;

      return rows * cols;
    }

    virtual bool done() {
      return file->eof() && (!linestream || !linestream->good());
    }

    unsigned getCols() {
      return cols; // 0 if unknown
    }

    unsigned getRows() {
      return rows; // rows read to so far
    }
  };
}

#endif
