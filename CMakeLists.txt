cmake_minimum_required(VERSION 3.6)
project(pHashRis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-function")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(PHASH_LIB_DIR /usr/local/lib)
link_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES main.cpp HashStore.cpp HashStore.h pHashRis.cpp pHashRis.h)
add_executable(pHashRis ${SOURCE_FILES})
target_link_libraries(pHashRis pHash)