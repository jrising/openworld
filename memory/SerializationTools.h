#ifndef SERIALIZATION_TOOLS_H
#define SERIALIZATION_TOOLS_H

#include <iostream>
#include <map>
#include <set>
#include <stdexcept>

using namespace std;

namespace openworld {
  class PointerTracker {
  protected:
    set<const void*> written;

  public:
    bool addPointer(const void* pointer) {
      if (written.find(pointer) == written.end()) {
	written.insert(pointer);
	return true; // this is a new pointer!
      }

      return false; // this is a previously added pointer
    }

    bool streamInsertPointer(ostream& os, const void* object) {
      os << object << " ";
      if (addPointer(object)) {
	os << "new" << endl;
	return true;
      } else {
	os << "old" << endl;
	return false;
      }
    }
  };

  class PointerReference {
  protected:
    map<void*, void*> old2new;

  public:
    void addPointer(void* oldp, void* newp) {
      old2new[oldp] = newp;
    }

    void* getPointer(void* oldp) {
      return old2new[oldp];
    }

    void* streamExtractPointer(istream& in, void* (*constructor)(istream&, PointerReference&)) {
      void* oldp;
      string oldornew;
      in >> oldp >> oldornew;
      if (oldornew == "new") {
	void* newp = constructor(in, *this);
	addPointer(oldp, newp);
	return newp;
      } else if (oldornew == "old")
	return getPointer(oldp);
      else
	throw invalid_argument("Expected 'old' or 'new' in streamExtractPointer.");
    }
  };

  class IPointerSerializable {
  public:
    virtual ostream& streamInsert(ostream& os, PointerTracker& tracker) const = 0;
    virtual istream& streamExtract(istream& in, PointerReference& reference) = 0;

    // Also define this, if every extracting from scratch:
    //static void* streamExtractPointer(istream& in, PointerReference& reference);

    ostream& streamInsertPointer(ostream& os, PointerTracker& tracker) const {
      if (tracker.streamInsertPointer(os, this))
	streamInsert(os, tracker);

      return os;
    }
  };
}

#endif
