#ifndef TRANSIENTS_H
#define TRANSIENTS_H

#include <iostream>
#include <list>

using namespace std;

#define tew_(call) openworld::Transients::track(new call)

namespace openworld {
  class IDeletable {
  public: 
    virtual ~IDeletable();

    virtual list<IDeletable*> getContainedPointers() const {
      return list<IDeletable*>();
    }
  };

  class Transients {
  protected:
    static list<IDeletable*> objects;
    static bool initialized;
    static bool cleaning;
    
  public:
    // Use any time memory is allocated: A* a = Memory::track(new A())
    template<class T>
    static T* track(T* obj) {
      if (!initialized) {
        objects = list<IDeletable*>();
        initialized = true;
      }

      objects.push_back(obj);
      return obj;
    }

    // might not be working!
    template<class T>
    static void abandon(T* obj) {
      if (!cleaning) {
        objects.remove((IDeletable*) obj);
        list<IDeletable*> ptrs = obj->getContainedPointers();
        list<IDeletable*>::iterator it;
        for (it = objects.begin(); it != objects.end(); it++)
          objects.remove(*it);
      }
    }

    static void clean() {
      cleaning = true;

      list<IDeletable*>::iterator it;
      for (it = objects.begin(); it != objects.end(); it++)
        delete *it;
      objects.clear();

      cleaning = false;
    }

    static unsigned count() {
      return objects.size();
    }
  };
}

#endif
