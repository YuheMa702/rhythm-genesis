#include "PythonBridge.hpp"
#include <Python.h>
#include <iostream>

bool PythonBridge::initialize(const std::string& pythonModulePath) {
    Py_Initialize();
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter" << std::endl;
        return false;
    }

    // Append the directory containing your Python modules to sys.path
    PyObject* sysPath = PySys_GetObject("path");
    PyObject* path = PyUnicode_FromString(pythonModulePath.c_str());
    if (PyList_Append(sysPath, path) != 0) {
        std::cerr << "Failed to append " << pythonModulePath << " to sys.path" << std::endl;
        Py_DECREF(path);
        return false;
    }
    Py_DECREF(path);
    return true;
}

void PythonBridge::finalize() {
    if (Py_IsInitialized()) {
        Py_Finalize();
    }
}

std::string PythonBridge::parseBeatmapFile(const std::string& filePath) {
    std::string result;
    
    // Import the Python module named "beatmap_parser"
    PyObject* moduleName = PyUnicode_FromString("beatmap_parser");
    PyObject* module = PyImport_Import(moduleName);
    Py_DECREF(moduleName);

    if (!module) {
        PyErr_Print();
        std::cerr << "Failed to load module 'beatmap_parser'" << std::endl;
        return "";
    }

    // Get the 'parse_audio_file' function from the module
    PyObject* func = PyObject_GetAttrString(module, "parse_audio_file");
    if (!func || !PyCallable_Check(func)) {
        PyErr_Print();
        std::cerr << "Failed to load function 'parse_audio_file'" << std::endl;
        Py_XDECREF(func);
        Py_DECREF(module);
        return "";
    }

    // Prepare a tuple to hold the function arguments (only one string argument in this case)
    PyObject* args = PyTuple_New(1);
    PyObject* arg = PyUnicode_FromString(filePath.c_str());
    // PyTuple_SetItem steals the reference to 'arg'
    PyTuple_SetItem(args, 0, arg);

    // Call the function with the provided arguments
    PyObject* pyResult = PyObject_CallObject(func, args);
    Py_DECREF(args);
    Py_DECREF(func);
    Py_DECREF(module);

    if (!pyResult) {
        PyErr_Print();
        std::cerr << "Failed to call function 'parse_audio_file'" << std::endl;
        return "";
    }

    // Ensure the result is a string and convert it to std::string
    if (PyUnicode_Check(pyResult)) {
        result = PyUnicode_AsUTF8(pyResult);
    } else {
        std::cerr << "Function 'parse_audio_file' did not return a string" << std::endl;
    }
    Py_DECREF(pyResult);
    
    return result;
}
