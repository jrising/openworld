/******************************************************************************\
 * ToString: Basic tools for converting elements to strings
 ******************************************************************************
 *
\******************************************************************************/

#ifndef TO_STRING_H
#define TO_STRING_H

#include <string>
#include <iostream>

using namespace std;

namespace openworld {
  class ToString {
  public:
    static string base10(int val) {
      stringstream strstr;
      strstr << val;
      return strstr.str();
    }
  };
}

#endif
