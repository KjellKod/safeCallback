cmake_minimum_required(VERSION 3.0)
project(callbackTest)

set(CMAKE_CXX_STANDARD 14)

#google test and google mock fused folder
set(gtestTopDir gtest)

include_directories(SYSTEM PUBLIC ${gtestTopDir}/gtest ${gtestTopDir}/gmock)
add_library(gtest STATIC ${gtestTopDir}/gmock-gtest-all.cc)

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
