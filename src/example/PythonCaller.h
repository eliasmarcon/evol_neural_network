// PythonCaller.h
#ifndef PYTHON_CALLER_H
#define PYTHON_CALLER_H

#include <Python.h>
#include <string>

class PythonCaller {
public:
    static PyObject* CallPythonFunction(const std::string &moduleName, 
                                        const std::string &functionName, 
                                        PyObject *args);
};

#endif // PYTHON_CALLER_H
