cmake_minimum_required(VERSION 3.0)
project(callbackTest)

set(CMAKE_CXX_STANDARD 14)
set(gtestDir ~/googletest/)

add_library(gtest STATIC ${gtestDir}/googletest/src/gtest-all.cc ${gtestDir}/googlemock/src/gmock-all.cc) 
include_directories(SYSTEM PUBLIC ${gtestDir}/googletest/include ${gtestDir}/googlemock/include)
include_directories(PUBLIC ${gtestDir}/googletest ${gtestDir}/googlemock)

set(testSrcs
    testMain
    caller
    service
)
add_executable(ut ${testSrcs})
target_link_libraries(ut gtest pthread)

add_custom_target(runStress COMMAND ut --gtest_repeat=1000 --gtest_break_on_failure DEPENDS ut)
