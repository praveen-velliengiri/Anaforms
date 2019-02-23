#minimal : create a .so lib, with llvm-config --cxxflags 
#don't link it with libs because it is done in opt tool
cmake_minimum_required(VERSION 3.0)
project(newpm)
find_package(LLVM REQUIRED CONFIG)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fPIC -fno-rtti -std=c++11")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
add_library(run MODULE Chains.cpp FunctionPass.cpp cfgpass.cpp)
