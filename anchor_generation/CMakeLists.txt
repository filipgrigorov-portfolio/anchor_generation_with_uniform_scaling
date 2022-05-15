cmake_minimum_required(VERSION 3.12)
project(anchors)

set(SRC src/anchor_generator.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/anchor_generator/include)