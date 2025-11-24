cmake_minimum_required(VERSION 3.16)
project(hvm LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/includes)

# ---- Shared library for common code ----
set(HVM_LIB_SOURCES
    #src/constant.cc
    # src/helper.cc
)

add_library(hvm_lib ${HVM_LIB_SOURCES})

target_include_directories(hasm_lib PUBLIC ${CMAKE_SOURCE_DIR}/includes)

# ---- Main executable ----
add_executable(hasm main.cc)

target_link_libraries(hvm PRIVATE hvm_lib)

# ---- Tests ----
enable_testing()
find_package(GTest REQUIRED)

add_executable(hvm_tests
    #   tests/test_parser.cc
    # tests/test_translate.cc
)

target_include_directories(hvm_tests PRIVATE ${CMAKE_SOURCE_DIR}/includes)

target_link_libraries(hvm_tests
    PRIVATE
        hvm_lib
        GTest::gtest
        GTest::gtest_main
)

