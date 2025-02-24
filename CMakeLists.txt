cmake_minimum_required(VERSION 3.16)
project(IrisEngine)

# Force using Clang++ cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .. && make -j$(nproc)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Find spdlog
find_package(spdlog REQUIRED)

# Add source files
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(IrisEngine ${SOURCES})

# Link SFML and spdlog
target_link_libraries(IrisEngine sfml-graphics sfml-window sfml-system spdlog::spdlog)

