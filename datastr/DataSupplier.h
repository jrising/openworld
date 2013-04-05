#ifndef DATA_SUPPLIER_H
#define DATA_SUPPLIER_H

#include "../ext/gzstream/gzstream.h"
#include <sys/stat.h>

namespace openworld {
  template<class T>
  class DataSupplier {
  public:
    virtual DataSupplier<T>* clone() = 0;

    virtual ~DataSupplier() {}

    virtual T get() = 0;
    virtual int length() = 0; // -1 for undetermined
    virtual bool done() = 0;
  };

  template<class T>
  class DelimitedSupplier : public DataSupplier<T> {
  protected:
    string filepath;

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
      this->filepath = filepath;

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

    virtual DataSupplier<T>* clone() {
      DelimitedSupplier<T>* copy = new DelimitedSupplier(filepath, parser, delimiter, allowblank);

      copy->file->seekg(file->tellg());
      if (linestream) {
        copy->linestream = new istringstream(linestream->str());
        copy->linestream->seekg(linestream->tellg());
      }

      copy->rows = rows;
      copy->cols = cols;
      copy->rowcols = rowcols;

      return copy;
    }

    virtual ~DelimitedSupplier() {
      delete file;
      if (linestream)
        delete linestream;
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
          cout << "  " << filepath << endl;
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

  template<class T>
  class BinarySupplier : public DataSupplier<T> {
  protected:
    string filepath;
    unsigned datasize;

    istream* file;

    T (*parser)(string);
    
  public:
    BinarySupplier(string filepath, unsigned datasize, T (*parser)(string) = NULL) {
      this->filepath = filepath;
      this->datasize = datasize;

      if (filepath.compare(filepath.length() - 3, 3, ".gz") == 0)
        file = new igzstream(filepath.c_str());
      else
        file = new ifstream(filepath.c_str());

      if (!file->good())
        throw runtime_error("Cannot open file");

      if (!parser)
        parser = FileParser<T>::parseNativeEndianFloat;
      this->parser = parser;
    }

    virtual DataSupplier<T>* clone() {
      BinarySupplier<T>* copy = new BinarySupplier(filepath, datasize, parser);

      copy->file->seekg(file->tellg());

      return copy;
    }

    virtual ~BinarySupplier() {
      delete file;
    }

    virtual T get() {
      if (!file->good()) {
        cout << "File done!" << endl;
        cout << "  " << filepath << endl;
        return T();
      }

      char val[datasize];
      file->read(val, datasize);

      return parser(val);
    }

    virtual int length() {
      struct stat filestatus;
      stat(filepath.c_str(), &filestatus);

      return filestatus.st_size / datasize;
    }

    virtual bool done() {
      return file->eof();
    }
  };
}

#endif
