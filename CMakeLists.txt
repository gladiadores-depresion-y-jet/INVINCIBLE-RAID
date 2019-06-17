cmake_minimum_required(VERSION 3.14)
project(RAID_5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

SET(OpenCV_DIR /home/dcamachog1501/OpenCV_installation/installation/OpenCV-master/lib/cmake/opencv4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
    # Add OpenCV headers location to your include paths
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()


add_executable(RAID_5
        Main/main.cpp
        RAID/RAID_Controller.cpp
        RAID/RAID_Controller.h
        Structures/List.h
        Structures/Node.h
        RAID/FilePart.cpp
        RAID/FilePart.h
        Huffman_Compressor/Huffman_Tree.cpp
        Huffman_Compressor/Huffman_Tree.h
        Huffman_Compressor/Compressor.cpp
        Huffman_Compressor/Compressor.h
        Huffman_Compressor/Huffman_Node.cpp
        Huffman_Compressor/Huffman_Node.h
        Huffman_Compressor/Huffman_Node.h
        Huffman_Compressor/Huffman_Node.h
        Huffman_Compressor/Huffman_Node.h
        RAID/Disk.cpp RAID/Disk.h
        server/requestHandler.h)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RAID_5 ${OpenCV_LIBS} ~/pistache/build/src/libpistache.so.0 curl)