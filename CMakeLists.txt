cmake_minimum_required(VERSION 3.0)
project(callbackTest)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#Test targets
set(testSrcs
    testMain
    caller
    service
)
add_executable(ut ${testSrcs})
target_link_libraries(ut ${GTEST_LIBRARIES} pthread)

#Simple runner
add_custom_target(runStress COMMAND ut --gtest_repeat=1000 --gtest_break_on_failure DEPENDS ut)
