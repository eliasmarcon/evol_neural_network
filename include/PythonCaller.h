// PythonCaller.h
#ifndef PYTHON_CALLER_H
#define PYTHON_CALLER_H

#include <Python.h>
#include <string>
#include <vector>

class PythonCaller {
public:
    PythonCaller();
    ~PythonCaller();

    static PyObject* CallPythonFunction(const std::string &moduleName, 
                                        const std::string &functionName, 
                                        PyObject *args);
    template <typename T>
    static PyObject* VectorToPyList(const std::vector<T>& vec);
    template <typename T>
    static PyObject* Vector2DToPyList(const std::vector<std::vector<T>>& vec);
};

#include "PythonCaller.tpp" // Include template implementation

// Inline template specializations
template <>
inline PyObject* ConvertToPyValue<int>(const int& value) {
    return PyLong_FromLong(static_cast<long>(value));
}

template <>
inline PyObject* ConvertToPyValue<long>(const long& value) {
    return PyLong_FromLong(value);
}

template <>
inline PyObject* ConvertToPyValue<double>(const double& value) {
    return PyFloat_FromDouble(value);
}

template <>
inline PyObject* ConvertToPyValue<std::string>(const std::string& value) {
    return PyUnicode_FromString(value.c_str());
}

#endif // PYTHON_CALLER_H
