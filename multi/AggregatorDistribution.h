#ifndef AGGREGATOR_DISTRIBUTION_H
#define AGGREGATOR_DISTRIBUTION_H

#include "Distribution.h"

namespace openworld {
  class AggregatorDistribution : Distribution {

    int callPython(TCHAR* title){
      PyObject *pName, *pModule, *pFunc;
      PyObject *pArgs, *pValue;

      Py_Initialize();
      pName = PyUnicode_FromString("Main");
      /* Name of Pythonfile */

      pModule = PyImport_Import(pName);
      Py_DECREF(pName);

      if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "writeLyricToFile");
        /* function name. pFunc is a new reference */
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(1);

            pValue = PyUnicode_FromWideChar(title, -1);

            if (!pValue) {
              Py_DECREF(pArgs);
              Py_DECREF(pModule);
              showErrorBox(_T("pValue is false"));
              return 1;
            }
            PyTuple_SetItem(pArgs, 0, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != NULL) {
              //worked as it should!
              Py_DECREF(pValue);
            }
            else {
              Py_DECREF(pFunc);
              Py_DECREF(pModule);
              PyErr_Print();
              showErrorBox(_T("pValue is null"));
              return 1;
            }
        }
        else {
          if (PyErr_Occurred()) PyErr_Print();
          showErrorBox(_T("pFunc null or not callable"));
          return 1;
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
      }
      else {
        PyErr_Print();
        showErrorBox(_T("pModule is null"));
        return 1;
      }
      Py_Finalize();
      return 0;
    }
  };
}

#endif
