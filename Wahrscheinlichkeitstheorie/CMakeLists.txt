cmake_minimum_required(VERSION 3.5)
project(Wahrscheinlichkeitstheorie)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES Wahrscheinlichkeitstheorie.cpp HoldemInitializer.cpp HoldemInitializer.h)
set(SOURCE_FILES Wahrscheinlichkeitstheorie.cpp Deck.cpp Deck.hpp)
add_executable(Wahrscheinlichkeitstheorie ${SOURCE_FILES})