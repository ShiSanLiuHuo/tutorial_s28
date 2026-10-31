cmake_minimum_required(VERSION 3.10)
project(ImaginaryNumber LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ImaginaryNumber
    imagnum.cpp
    use_imagnum.cpp
)