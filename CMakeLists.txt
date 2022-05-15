cmake_minimum_required(VERSION 3.12)
project(main)

set(SRC main.cpp)

add_subdirectory(anchor_generator)

include_directories(${PROJECT_SOURCE_DIR}/anchor_generation/include)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC anchor_generator)
