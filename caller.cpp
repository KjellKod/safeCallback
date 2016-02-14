#include  "caller.h"

using namespace std;

void Caller::start(){
    runner_ = std::thread(&Caller::run, this);
}

void Caller::stop(){
    runner_.join();
}

void Caller::registerFor(int type, Callable cb){
    lock_guard<mutex> lock(mutex_);
    callbacks_[type] = cb;
}

void Caller::unregister(int type){
    {
        lock_guard<mutex> lock(mutex_);
        callbacks_.erase(type);
    }

    if (afterUnReg_){
        afterUnReg_();
    }
}

void Caller::run(){
    auto done = false; 
    Callable cb;

    while (!done){
        {
            lock_guard<mutex> lock(mutex_);
            if (callbacks_.empty())
                break;
            cb = callbacks_.begin()->second;
            callbacks_.erase(callbacks_.begin());
            done = callbacks_.size();
        }

        if (before_) before_();
        cb();
        if (after_) after_();
    }
}

void Caller::setProbe(Callable beforeRun, Callable afterRun, Callable afterUnReg){
    before_ = beforeRun;
    after_ = afterRun;
    afterUnReg_ = afterUnReg;
}
