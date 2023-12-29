#include <Python.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    Py_Initialize();

    // Set the Python path to include the current directory
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    // Import the Python script
    PyObject *pName = PyUnicode_DecodeFSDefault("greet");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, "greet");
        
        if (pFunc && PyCallable_Check(pFunc)) {
            // Get name from command line arguments or default to "World"
            std::string name = "World";
            if (argc > 1) {
                name = argv[1];
            }
            PyObject *pArgs = PyTuple_New(1);
            PyObject *pValue = PyUnicode_FromString(name.c_str());
            PyTuple_SetItem(pArgs, 0, pValue);

            // Call the function
            PyObject *pResult = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pResult != nullptr) {
                // Convert the returned value to C++ string and print
                std::cout << PyUnicode_AsUTF8(pResult) << std::endl;
                Py_DECREF(pResult);
            } else {
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

    // Clean up and shut down the Python Interpreter
    Py_Finalize();
    return 0;
}
