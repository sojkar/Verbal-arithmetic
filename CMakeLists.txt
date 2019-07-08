cmake_minimum_required (VERSION 3.5)
project (verbalArithmetic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(MiniSat 2.2 REQUIRED)

add_executable(verbalArithmetic
    main.cpp
    Solver.cpp
    # Headers for IDEs
    Solver.h
)
target_link_libraries(verbalArithmetic MiniSat::libminisat)
