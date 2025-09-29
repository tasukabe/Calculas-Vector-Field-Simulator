cmake_minimum_required(VERSION 3.20)

project(CVFR VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(raylib CONFIG REQUIRED)

add_executable(CVFR 
	src/Main.cpp
	src/Representator.cpp
	src/Representator.hpp
	#src/ParsingEquation.hpp
	#src/ParsingEquation.cpp
)

target_link_libraries(CVFR PRIVATE raylib)

