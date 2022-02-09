cmake_minimum_required(VERSION 3.21)
project(testServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(testServer main.cpp packer.cpp packer.h base64.cpp base64.h)
if(WIN32)
    target_link_libraries(testServer ws2_32)
endif()

find_package(OpenCV REQUIRED)
find_package (Threads REQUIRED)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wunused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -flto -funroll-loops -fvect-cost-model=unlimited")

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        ${OpenCV_LIBS}
        )