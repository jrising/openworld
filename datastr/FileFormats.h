#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <string>

using namespace std;

template<class T>
class FileParser {
 public:
  static T parseSimple(string elt) {
    if (elt[0] == '"')
      return (T) atof(elt.c_str() + 1);
    return (T) atof(elt.c_str());
  }
};

template<class T>
class FileFormatter {
 public:
  static string formatSimple(T num) {
    char result[20];

    T isfloat = (T) (num + .5);
    if (isfloat == num)
      sprintf(result, "%ld", (long) num);
    else
      sprintf(result, "%lf", (double) num);

    return result;
  }
};

#endif
