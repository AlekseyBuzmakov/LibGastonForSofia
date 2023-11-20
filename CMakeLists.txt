cmake_minimum_required(VERSION 3.5)
project(LibGastonForSofia LANGUAGES CXX)

FILE(GLOB CPP_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${CPP_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc/ ${PROJECT_SOURCE_DIR}/src/)
target_link_libraries(${PROJECT_NAME} PUBLIC)

