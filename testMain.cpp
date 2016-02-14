#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <chrono>
#include <atomic>
#include "service.h"
#include "caller.h"

using namespace std;
using namespace std::chrono_literals;

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(safeDelete, invokedAfterUnregisteredAndDestroy_callbackBodySkipped){
    Caller caller;
    auto service = make_shared<Service>(caller);
    std::atomic<bool> start{false}, end{false};

    //make sure unregister call in destructor is late enough
    caller.setProbe([&start](){
       start = true; 
       this_thread::sleep_for(30ms);
    }, [&end](){ end = true; });

    caller.start();
    while (!start){
        this_thread::sleep_for(10ms);
    }

    ASSERT_FALSE(end);
    service = nullptr; //unregiter here in destructor

    caller.stop();
    EXPECT_THAT(Service::probeMarker, ::testing::ElementsAre(0,1,2,3,11));
}

TEST(safeDelete, invokedJustAfterUnregisterBeforeDestroy_callbackExecuted){
    Caller caller;
    auto service = make_shared<Service>(caller);
    std::atomic<bool> start{false}, end{false};

    caller.setProbe([&start](){
        //sync until destructor is called first
        start = true; 
        this_thread::sleep_for(20ms);
    }, [&end](){ end = true; }, [](){
        //after unregister, defer resource destroy
        this_thread::sleep_for(40ms);
    });

    caller.start();
    while (!start){
        this_thread::sleep_for(10ms);
    }

    service = nullptr; //unregiter here in destructor

    caller.stop();
    EXPECT_THAT(Service::probeMarker, ::testing::ElementsAre(0,11,12,1,2,3));
}

TEST(unManaged, interleavedDestroyAndCall_noCrashUnderStress){
    Caller caller;
    auto service = make_shared<Service>(caller);
    caller.setProbe(Callable(), Callable(), Callable());

    caller.start();
    service = nullptr;
    caller.stop();
}
