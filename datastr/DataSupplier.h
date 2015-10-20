#ifndef DATA_SUPPLIER_H
#define DATA_SUPPLIER_H

#ifndef SKIP_GZSTREAM
#include "../ext/gzstream/gzstream.h"
#endif
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

#ifndef SKIP_GZSTREAM
      if (filepath.compare(filepath.length() - 3, 3, ".gz") == 0)
        file = new igzstream(filepath.c_str());
      else
        file = new ifstream(filepath.c_str());
#else
      file = new ifstream(filepath.c_str());
#endif

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

#ifndef SKIP_GZSTREAM
      if (filepath.compare(filepath.length() - 3, 3, ".gz") == 0)
        file = new igzstream(filepath.c_str());
      else
        file = new ifstream(filepath.c_str());
#else
      file = new ifstream(filepath.c_str());
#endif

      if (!file->good()) {
        cout << "Cannot open file: " << filepath << endl;
        throw runtime_error("Cannot open file");
      }

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

  template<class T, class S>
  class ScaleSupplier : public DataSupplier<T> {
  protected:
    DataSupplier<S>* source; // own this
    double scale;
  public:
    ScaleSupplier(DataSupplier<S>* source, double scale) {
      this->source = source;
      this->scale = scale;
    }

    virtual DataSupplier<T>* clone() {
      return new ScaleSupplier(source->clone(), scale);
    }

    virtual ~ScaleSupplier() {
      delete source;
    }

    virtual T get() {
      return (T) (source->get() * scale);
    }

    virtual int length() {
      return source->length();
    }

    virtual bool done() {
      return source->done();
    }
  };

  template<class T, class S>
  class SumSupplier : public DataSupplier<T> {
  protected:
    DataSupplier<S>* source1; // own this
    DataSupplier<S>* source2; // own this
  public:
    SumSupplier(DataSupplier<S>* source1, DataSupplier<S>* source2) {
      this->source1 = source1;
      this->source2 = source2;
    }

    virtual DataSupplier<T>* clone() {
      return new SumSupplier<T, S>(source1->clone(), source2->clone());
    }

    virtual ~SumSupplier() {
      delete source1;
      delete source2;
    }

    virtual T get() {
      return (T) (source1->get() + source2->get());
    }

    virtual int length() {
      return min(source1->length(), source2->length());
    }

    virtual bool done() {
      return source1->done() || source2->done();
    }
  };

  template<class T>
  class AddSupplier : public DataSupplier<T> {
  protected:
    DataSupplier<T>* source; // own this
    T add;
  public:
    AddSupplier(DataSupplier<T>* source, T add) {
      this->source = source;
      this->add = add;
    }

    virtual DataSupplier<T>* clone() {
      return new AddSupplier<T>(source->clone(), add);
    }

    virtual ~AddSupplier() {
      delete source;
    }

    virtual T get() {
      return source->get() + add;
    }

    virtual int length() {
      return source->length();
    }

    virtual bool done() {
      return source->done();
    }
  };
}

#endif
