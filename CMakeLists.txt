cmake_minimum_required(VERSION 3.0)
project(callbackTest)

set(CMAKE_CXX_STANDARD 14)

#google test and google mock
set(gtestTopDir /usr/ CACHE PATH
    "google test and google mock install directory prefix")

add_library(gtest STATIC ${gtestTopDir}/src/gtest/src/gtest-all.cc ${gtestTopDir}/src/gmock/src/gmock-all.cc) 
include_directories(SYSTEM PUBLIC ${gtestTopDir}/include/gtest/include ${gtestTopDir}/include/gmock/include)
target_include_directories(gtest PUBLIC ${gtestTopDir}/src/gtest ${gtestTopDir}/src/gmock)

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
