#include "service.h"
#include "caller.h"

using namespace std;

std::vector<int> Service::probeMarker{};

Service::Service(Caller& c) : caller_(c){
    probeMarker.clear(); //only track one instance actually

    sync_ = make_shared<int>(0);
    resource_ = make_unique<int>(0);
    std::weak_ptr<int> ref(sync_);
    caller_.registerFor(0, [this, ref = std::move(ref)](){
            doSomething(ref);
            });
}

Service::~Service(){
    probeMarker.push_back(0);
    caller_.unregister(0); 
    probeMarker.push_back(1);
    sync_ = nullptr;
    probeMarker.push_back(2);

    lock_guard<mutex> lock(mutex_);
    resource_ = nullptr;
    probeMarker.push_back(3);
}

void Service::doSomething(std::weak_ptr<int> ref){
    probeMarker.push_back(11);
    if (!(ref.expired())){
        lock_guard<mutex> lock(mutex_);
        *resource_ = 123;
        probeMarker.push_back(12);
    }
}
