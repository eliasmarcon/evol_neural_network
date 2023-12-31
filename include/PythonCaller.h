// PythonCaller.h
#ifndef PYTHON_CALLER_H
#define PYTHON_CALLER_H

#include <Python.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

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

// Inline ConvertToPyValue specializations
template <typename T>
inline PyObject* ConvertToPyValue(const T& value) {
    // Dummy implementation - replace with type-specific conversion
    return Py_None;
}

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

// Template function definitions
template <typename T>
PyObject* PythonCaller::VectorToPyList(const std::vector<T>& vec) {
    PyObject* pList = PyList_New(vec.size());
    if (!pList) throw std::runtime_error("Failed to create Python list");

    for (size_t i = 0; i < vec.size(); ++i) {
        PyObject* pValue = ConvertToPyValue(vec[i]);
        if (!pValue) {
            Py_DECREF(pList);
            throw std::runtime_error("Failed to create Python object from vector element");
        }
        PyList_SetItem(pList, i, pValue); // PyList_SetItem steals reference to pValue
    }
    return pList;
}

template <typename T>
PyObject* PythonCaller::Vector2DToPyList(const std::vector<std::vector<T>>& vec) {
    std::cout<<"Vector2DToPyList"<<std::endl;
    PyObject* pList = PyList_New(vec.size());
    if (!pList) throw std::runtime_error("Failed to create Python list for 2D vector");

    for (size_t i = 0; i < vec.size(); ++i) {
        PyObject* pInnerList = PyList_New(vec[i].size());
        if (!pInnerList) {
            Py_DECREF(pList);
            throw std::runtime_error("Failed to create inner Python list for 2D vector");
        }

        for (size_t j = 0; j < vec[i].size(); ++j) {
            PyObject* pValue = ConvertToPyValue(vec[i][j]);
            if (!pValue) {
                Py_DECREF(pInnerList);
                Py_DECREF(pList);
                throw std::runtime_error("Failed to create Python object from 2D vector element");
            }
            PyList_SetItem(pInnerList, j, pValue); // PyList_SetItem steals reference to pValue
        }

        PyList_SetItem(pList, i, pInnerList); // PyList_SetItem steals reference to pInnerList
    }
    return pList;
}

#endif // PYTHON_CALLER_H
