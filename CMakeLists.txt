# cmake_minimum_required(VERSION 3.16)
# project(IrisEngine)

# # Force using Clang++
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)

# # Set C++ standard
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Find SFML
# find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# # Find spdlog
# find_package(spdlog REQUIRED)

# # Find Python3
# find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
# include_directories(${Python3_INCLUDE_DIRS})

# # Add tinyfiledialogs library (assumes tinyfiledialogs is in external/tinyfiledialogs)
# add_library(tinyfiledialogs external/tinyfiledialogs/tinyfiledialogs.c)
# target_include_directories(tinyfiledialogs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyfiledialogs)


# # Add source files
# file(GLOB_RECURSE SOURCES src/*.cpp 
#      src/UI/*.cpp)
# add_executable(IrisEngine ${SOURCES})

# # Link SFML, spdlog, and pybind11 (embed module for embedding Python)
# target_link_libraries(IrisEngine sfml-graphics sfml-window sfml-system sfml-audio spdlog::spdlog ${Python3_LIBRARIES} tinyfiledialogs)
cmake_minimum_required(VERSION 3.16)
project(IrisEngine)

# Force using Clang++
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Find SFML (explicitly include audio!)
# find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
find_package(SFML COMPONENTS system REQUIRED) # This will fix openAL internal errors hopefully
message(STATUS "SFML version: ${SFML_VERSION}")

# Find spdlog
find_package(spdlog REQUIRED)

# Find Python3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

# Add tinyfiledialogs library (assumes tinyfiledialogs is in external/tinyfiledialogs)
add_library(tinyfiledialogs external/tinyfiledialogs/tinyfiledialogs.c)
target_include_directories(tinyfiledialogs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyfiledialogs)

# Add source files
file(GLOB_RECURSE SOURCES src/*.cpp src/UI/*.cpp)
add_executable(IrisEngine ${SOURCES})

# Link SFML, spdlog, and Python3
target_link_libraries(IrisEngine
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    spdlog::spdlog
    ${Python3_LIBRARIES}
    tinyfiledialogs
)
