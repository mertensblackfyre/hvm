cmake_minimum_required(VERSION 3.23)
project(hvm LANGUAGES C CXX)

# include_directories(${CMAKE_SOURCE_DIR}/includes)
add_executable(hvm)
target_sources(hvm PRIVATE main.cc)

add_library(hvm_lib)

target_sources(
  hvm_lib
  PRIVATE src/
  PUBLIC FILE_SET HEADERS includes/*)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(hvm PRIVATE hvm_lib)
# target_link_libraries(hvm fmt spdlog hvm_lib)

# enable_testing() add_executable(hvm_tests tests/test_parser.cc
# tests/test_translate.cc )

# target_include_directories(hvm_tests PRIVATE ${CMAKE_SOURCE_DIR}/includes)
