cmake_minimum_required(VERSION 3.14)
set(projectName IoTTest)
project(${projectName})

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(DOWNLOAD_EXTRACT_TIMESTAMP true)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
FetchContent_MakeAvailable(googletest)

message("Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCES_TEST
    ../src/implementation/co2Impl/co2impl.c
    ../src/implementation/tempImpl/temperaturImpl.c
    ../src/implementation/humidityImpl/humidityImpl.c
    ../src/handlers/servoHandler/servoHandler.c
    ../src/handlers/co2Handler/co2Handler.c
    test_production.cpp
    servoTest.cpp
    FreeRTOS_defs/FreeRTOS_FFF_MocksDefinitions.cpp
    ${SRCS})

# This makes the test.cpp files able to #include target libraries without errors. 
include_directories(
    ${CMAKE_SOURCE_DIR}
    ../drivers
    FreeRTOS_defs
    ../src/implementation/co2Impl
    ../src/implementation/tempImpl
    ../src/implementation/tempImpl/delay_hal
    ../src/implementation/humidityImpl
    ../src
    ../interfaceWrapper
    ../src/handlers/servoHandler
    ../src/handlers/co2Handler
    
  
    ../src/DataCollection
    )

# Something to 
enable_testing()

add_executable(${projectName} ${SOURCES_TEST})

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif(MSVC)

target_link_libraries(${projectName} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${projectName})