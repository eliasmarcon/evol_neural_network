// main.cpp
#include "PythonCaller.h"
#include <iostream>
#include <vector>

int main() {
    std::cout <<"start"<< std::endl;
    std::vector<int> myArray = {1, 2, 3, 4, 5}; // Example array

// Ensure Python interpreter is initialized
    Py_Initialize();

    PyObject* pList = PyList_New(myArray.size());
    if (!pList) {
        std::cerr << "Failed to create Python list" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < myArray.size(); ++i) {
        PyObject* pValue = PyLong_FromLong(myArray[i]);
        if (!pValue) {
            std::cerr << "Failed to create Python integer" << std::endl;
            Py_DECREF(pList);
            return 1;
        }
        PyList_SetItem(pList, i, pValue); // Note: PyList_SetItem steals reference
    }

    // Create a tuple and set the list as the single item in the tuple
    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, pList); // Note: PyTuple_SetItem steals reference to pList

    PyObject* pResult = PythonCaller::CallPythonFunction("array_processor", "process_array", pArgs);
    if (pResult != nullptr) {
        // Assuming the function returns a list, print each element
        if (PyList_Check(pResult)) {
            for (Py_ssize_t i = 0; i < PyList_Size(pResult); ++i) {
                PyObject* pItem = PyList_GetItem(pResult, i); // Borrowed reference
                if (PyLong_Check(pItem)) {
                    std::cout << PyLong_AsLong(pItem) << " ";
                }
            }
            std::cout << std::endl;
        }
        Py_DECREF(pResult);
    }

    Py_DECREF(pArgs);
    Py_Finalize();
    return 0;
}
