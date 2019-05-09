cmake_minimum_required(VERSION 3.14)
project(BigNum)

set(CMAKE_CXX_STANDARD 17)


add_library(BigNum
        src/BigInteger.cpp
        src/BigInteger.h
        )

add_executable(BigNumTest RunTests.cpp)
target_link_libraries(BigNumTest gtest)
target_link_libraries(BigNumTest BigNum)
