cmake_minimum_required(VERSION 2.8.8)
project(DisplayLiFiLib)

include_directories(
    "test/build/arduino_mock/arduino_mock/src/arduino_mock/include/arduino-mock"
    "test/build/arduino_mock/arduino_mock/src/arduino_mock-build/lib/gmock/gmock/src/gmock/include"
    "test/build/arduino_mock/arduino_mock/src/arduino_mock-build/lib/gtest/gtest/src/gtest/include"
    )

file(GLOB SRC "*.cpp")

add_subdirectory(test)