cmake_minimum_required(VERSION 3.29)
project(PA3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PA3
    Employee.h
    LinearProbing.h
    main.cpp
    SeparateChaining.h
    testLinearProbing.h
    testSeparateChaining.cpp
    testSeparateChaining.h
    utils.cpp
    utils.h
        testLinearProbing.cpp)
