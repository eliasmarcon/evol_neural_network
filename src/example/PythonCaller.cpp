// PythonCaller.cpp
#include "PythonCaller.h"

PyObject* PythonCaller::CallPythonFunction(const std::string &moduleName, 
                                           const std::string &functionName, 
                                           PyObject *args) {
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    PyObject *pName = PyUnicode_FromString(moduleName.c_str());
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    PyObject *pResult = nullptr;

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
        if (pFunc && PyCallable_Check(pFunc)) {
            pResult = PyObject_CallObject(pFunc, args);
            if (pResult == nullptr) {
                PyErr_Print();
            }
        } else {
            if (PyErr_Occurred())
                PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }

    return pResult;
}
