#include "PythonCaller.h"
#include <iostream>
#include <vector>

int main() {
    PythonCaller pyCaller;
    
    // Example 2D vector
    std::vector<std::vector<float>> my2DVector = {
        {1.234f, 2.1434f, 3.0f},
        {4.0f, 5.0f, 6.43263456656540f}
    };

    PyObject* pList = pyCaller.Vector2DToPyList(my2DVector);
    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, pList);
    PyObject* result = pyCaller.CallPythonFunction("array_processor", "process_2d_array", pArgs);
    if (result != nullptr) {
        // Process result
    }

    return 0;
}
