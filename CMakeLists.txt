cmake_minimum_required(VERSION 3.16)
project(orbifold VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(orbifold_lib
    src/OrbifoldSymbol.cpp
)

target_include_directories(orbifold_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(orbifold_lib PUBLIC cxx_std_20)

add_executable(orbifold_example src/main.cpp)
target_link_libraries(orbifold_example PRIVATE orbifold_lib)

add_executable(orbifold_tests tests/OrbifoldSymbolTests.cpp)
target_link_libraries(orbifold_tests PRIVATE orbifold_lib)
add_test(NAME OrbifoldSymbolTests COMMAND orbifold_tests)

include(CTest)
