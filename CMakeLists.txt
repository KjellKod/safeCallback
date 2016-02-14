cmake_minimum_required(VERSION 3.0)
project(callbackTest)

set(CMAKE_CXX_STANDARD 14)

#google test and google mock
set(gtestDir ~/googletest/ CACHE PATH 
    "google test repository top path (containing googltest/googlemock), shall be checked out from github: https://github.com/google/googletest/")

add_library(gtest STATIC ${gtestDir}/googletest/src/gtest-all.cc ${gtestDir}/googlemock/src/gmock-all.cc) 
include_directories(SYSTEM PUBLIC ${gtestDir}/googletest/include ${gtestDir}/googlemock/include)
include_directories(PUBLIC ${gtestDir}/googletest ${gtestDir}/googlemock)

#Test targets
set(testSrcs
    testMain
    caller
    service
)
add_executable(ut ${testSrcs})
target_link_libraries(ut gtest pthread)

#Simple runner
add_custom_target(runStress COMMAND ut --gtest_repeat=1000 --gtest_break_on_failure DEPENDS ut)
