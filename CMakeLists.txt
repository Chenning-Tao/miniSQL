cmake_minimum_required(VERSION 3.19)
project(miniSQL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(miniSQL main.cpp Interpreter.cpp Interpreter.h CommonHead.h API.cpp API.h Utility/CatalogManager.cpp Utility/CatalogManager.h Utility/BufferManager.cpp Utility/BufferManager.h Utility/Table.cpp Utility/Table.h Utility/RecordManager.cpp Utility/RecordManager.h Utility/tabulate.hpp Utility/BpTree.h Utility/IndexManager.cpp Utility/IndexManager.h Utility/BpTree.cpp)