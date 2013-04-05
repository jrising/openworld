#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <string>
#include <typeinfo>

using namespace std;

template<class T>
class FileParser {
 public:
  static T parseSimple(string elt) {
    if (elt[0] == '"')
      return (T) atof(elt.c_str() + 1);
    return (T) atof(elt.c_str());
  }

  static T parseNativeEndianFloat(string elt) {
    return *((T*) elt.c_str());
  }

  static T parseSwapEndianFloat(string elt) {
    char swap[elt.length()];
    for (unsigned ii = 0; ii < elt.length(); ii++)
      swap[elt.length() - ii - 1] = elt[ii];
    return *((T*) swap);
  }
};

template<class T>
class FileFormatter {
 public:
  static string formatSimple(T num) {
    bool checkbool;
    short checkshort;
    int checkint;
    char result[20];

    if (typeid(checkbool) == typeid(num))
      sprintf(result, "%d", (int) num);      
    else if (typeid(checkshort) == typeid(num) || typeid(checkint) == typeid(num))
      sprintf(result, "%ld", (long) num);
    else
      snprintf(result, 20, "%f", (float) num);

    return result;
  }
};

#endif
