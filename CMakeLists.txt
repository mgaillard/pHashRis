cmake_minimum_required(VERSION 3.6)
project(pHashRis)

option(WITH_NATIVE_ARCH "Compile in 64 bits for the native architecture." OFF)
option(WITH_POPCNT "Make use of the POPCOUNT instruction of the SSE instruction set. Depends on the WITH_NATIVE_ARCH option." OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-function")
if(WITH_NATIVE_ARCH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -march=native")
endif()
if(WITH_POPCNT)
    add_definitions(-DPOPCNT)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost COMPONENTS program_options REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(PHASH_LIB_DIR /usr/local/lib)
link_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES main.cpp HashStore.cpp HashStore.h pHashRis.cpp pHashRis.h Hamming.h Hamming.cpp)
add_executable(pHashRis ${SOURCE_FILES})
target_link_libraries(pHashRis ${Boost_LIBRARIES})
target_link_libraries(pHashRis pHash)