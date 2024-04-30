cmake_minimum_required(VERSION 3.25)
project(project)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
find_package(OpenCV REQUIRED)

add_executable(project main.cpp)

if (OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif ()

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})