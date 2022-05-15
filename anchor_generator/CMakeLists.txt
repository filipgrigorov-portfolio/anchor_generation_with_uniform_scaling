cmake_minimum_required(VERSION 3.12)

set(SRC src/anchor_generator.cpp)

add_library(anchor_generator ${SRC})
target_include_directories(anchor_generator PUBLIC ${CMAKE_SOURCE_DIR}/anchor_generator/include)
