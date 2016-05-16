#include "service.h"
#include "caller.h"

using namespace std;

std::vector<int> Service::probeMarker{};

struct Service::SyncHolder{
    std::mutex mutex;
    bool       released {false};
};

Service::Service(Caller& c) : caller_(c){
    probeMarker.clear(); //only track one instance actually

    sync_ = make_shared<SyncHolder>();
    resource_ = make_unique<int>(0);
    std::weak_ptr<SyncHolder> ref(sync_);
    caller_.registerFor(0, [this, ref = std::move(ref)](){
            doSomething(ref);
            });
}

Service::~Service(){
    probeMarker.push_back(0);
    caller_.unregister(0); 

    probeMarker.push_back(1);
    std::weak_ptr<SyncHolder> ref(sync_);
    sync_ = nullptr;
    probeMarker.push_back(2);

    if (!ref.expired()){
        auto sync = ref.lock();
        lock_guard<mutex> lock(sync->mutex);
        resource_ = nullptr;
        sync->released = true;
    }else{
        resource_ = nullptr;
    }
    probeMarker.push_back(3);
}

void Service::doSomething(std::weak_ptr<SyncHolder> ref){
    probeMarker.push_back(11);
    if (auto sync = ref.lock()){
        lock_guard<mutex> lock(sync->mutex);
        if (!(sync->released)){
            //possibly dtor obtain mutex first
            *resource_ = 123;
        }
        probeMarker.push_back(12);
    }
}
