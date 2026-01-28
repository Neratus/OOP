cmake_minimum_required (VERSION 3.8)


project(
    OOP_Lab_02
    DESCRIPTION "Lab 02 for BMSTU OOP Course, ICS7"
    LANGUAGES CXX
)
cmake_minimum_required(VERSION 3.5)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(
    _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
)


set(SRC_DOMAIN src)
set(src
        ${SRC_DOMAIN}/ConstIterator.hpp
        ${SRC_DOMAIN}/Iterator.hpp
        ${SRC_DOMAIN}/Matrix.hpp
        ${SRC_DOMAIN}/MatrixRow.hpp
        ${SRC_DOMAIN}/BaseMatrix.cpp
        "src/MatrixPrivate.hpp")

set(INC_DOMAIN inc)
set(inc
        ${INC_DOMAIN}/BaseMatrix.h
        ${INC_DOMAIN}/ConstIterator.h
        ${INC_DOMAIN}/Iterator.h
        ${INC_DOMAIN}/Matrix.h
        ${INC_DOMAIN}/Concepts.hpp
        ${INC_DOMAIN}/Exceptions.h)

include_directories(
        ${SRC_DOMAIN}
        ${INC_DOMAIN})


add_executable(lab_02
        main.cpp
        ${INC}
        ${SRC} "src/MatrixMath.hpp" "src/MatrixTransformations.hpp" "src/BaseMatrix.cpp")
