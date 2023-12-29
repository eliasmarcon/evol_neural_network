// PythonCaller.tpp
#ifndef PYTHON_CALLER_TPP
#define PYTHON_CALLER_TPP

#include <stdexcept>

// Generic converter function
template <typename T>
PyObject* PythonCaller::ConvertToPyValue(const T& value);

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

#endif // PYTHON_CALLER
