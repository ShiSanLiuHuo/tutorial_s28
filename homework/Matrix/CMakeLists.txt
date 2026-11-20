CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(Matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Matrix src/main.cpp src/Matrix.cpp)

target_include_directories(Matrix PRIVATE
	${PROJECT_SOURCE_DIR}/include
)


