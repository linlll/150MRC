cmake_minimum_required(VERSION 3.10)

project(MAIN)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

set(DATA_GENERATOR_SRCS data_generator.cpp)
set(
  READ_AND_CALCULATE_SRCS
  read_and_calculate.cpp
)
set(ANALYSIS_LOG_SRCS analysis_log.cpp)

add_executable(data_generator ${DATA_GENERATOR_SRCS})
add_executable(read_and_calculate ${READ_AND_CALCULATE_SRCS})
add_executable(analysis_log ${ANALYSIS_LOG_SRCS})
