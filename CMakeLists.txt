cmake_minimum_required(VERSION 3.12)
project(main)

set(${SRC} main.cpp)

add_subdirectory(anchor_generator)

add_executable(${PROJECT_NAME} PUBLIC ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC anchor_generator)