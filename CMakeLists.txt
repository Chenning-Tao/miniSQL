cmake_minimum_required(VERSION 3.19)
project(miniSQL)

set(CMAKE_CXX_STANDARD 17)

add_executable(miniSQL main.cpp Interpreter.cpp Interpreter.h CommonHead.h API.cpp API.h Utility/CatalogManager.cpp Utility/CatalogManager.h Utility/BufferManager.cpp Utility/BufferManager.h Utility/Table.cpp Utility/Table.h Utility/RecordManager.cpp Utility/RecordManager.h Utility/tabulate.hpp)