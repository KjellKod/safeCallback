cmake_minimum_required(VERSION 3.0)
project(callbackTest)

set(CMAKE_CXX_STANDARD 14)
include_directories(SYSTEM PUBLIC gtest)

#Test targets
set(testSrcs
    testMain
    caller
    service
    gtest/gmock-gtest-all.cc
)
add_executable(ut ${testSrcs})
target_link_libraries(ut pthread)

#Simple runner
add_custom_target(runStress COMMAND ut --gtest_repeat=1000 --gtest_break_on_failure DEPENDS ut)
