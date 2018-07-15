cmake_minimum_required(VERSION 3.9)

project(categories_classifier)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(SOURCE_FILES
        src/main.cpp
        src/program.cpp
        src/program.hpp
        )

add_executable(categories_classifier ${SOURCE_FILES})
target_link_libraries(categories_classifier -lspeed -lstdc++fs)
install(TARGETS categories_classifier DESTINATION bin)
