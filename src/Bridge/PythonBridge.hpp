#ifndef PYTHON_BRIDGE_H
#define PYTHON_BRIDGE_H

#include <string>

class PythonBridge {
public:
    // Initializes the Python interpreter and appends the given directory (e.g., "src/python")
    // to sys.path so that Python modules can be found.
    static bool initialize(const std::string& pythonModulePath);

    // Finalizes the Python interpreter.
    static void finalize();

    // Calls the Python function 'parse_audio_file' from the module 'beatmap_parser'
    // and returns its JSON string result.
    static std::string parseBeatmapFile(const std::string& filePath);
};

#endif // PYTHON_BRIDGE_H
