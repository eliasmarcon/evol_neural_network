// PythonCaller.cpp
#include "PythonCaller.h"


// Explicit instantiations of ConvertToPyValue for required types
template PyObject* ConvertToPyValue<int>(const int&);
template PyObject* ConvertToPyValue<long>(const long&);
template PyObject* ConvertToPyValue<double>(const double&);
template PyObject* ConvertToPyValue<std::string>(const std::string&);
template PyObject* ConvertToPyValue<float>(const float&);

PythonCaller::PythonCaller() {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    // PyRun_SimpleString("print(sys.version)");
    // PyRun_SimpleString("print(sys.executable)");
    // PyRun_SimpleString("help('modules')");
    PyRun_SimpleString("sys.path.append('./src')");
}


PythonCaller::~PythonCaller() {
    Py_Finalize();
}

PyObject* PythonCaller::CallPythonFunction(const std::string &moduleName, 
                                           const std::string &functionName, 
                                           PyObject *args,
                                           float& loss, 
                                           float& acc) {

    // Import the Python script
    PyObject *pName = PyUnicode_FromString(moduleName.c_str());
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    PyObject *pResult = nullptr;

    if (pModule != nullptr) {
        PyObject *pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
        if (pFunc && PyCallable_Check(pFunc)) {
            pResult = PyObject_CallObject(pFunc, args);
            // Extract test_loss and test_acc from the returned tuple
            if (pResult != NULL && PyTuple_Check(pResult) && PyTuple_Size(pResult) == 2) {
                loss = PyFloat_AsDouble(PyTuple_GetItem(pResult, 0));
                acc = PyFloat_AsDouble(PyTuple_GetItem(pResult, 1));
            } else {
                PyErr_Print();
            }
            Py_XDECREF(pResult);
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
