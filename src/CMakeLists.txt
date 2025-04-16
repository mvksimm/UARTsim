cmake_minimum_required(VERSION 3.10)
project(UARTSensorProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(UARTSensor
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/uart_sensor.cpp
)

target_include_directories(UARTSensor PRIVATE ${CMAKE_SOURCE_DIR}/../include)