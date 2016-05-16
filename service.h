#include <functional>
#include <memory>
#include <mutex>
#include <vector>

class Caller;
class Service{
    private:
        struct SyncHolder;

    public:
        Service(Caller& caller);
        Service& operator=(const Service&) = delete;
        ~Service();
        
        void doSomething(std::weak_ptr<SyncHolder> ref);

        //testing purpose only
        static std::vector<int> probeMarker;

    private:
        std::shared_ptr<SyncHolder> sync_;
        std::unique_ptr<int> resource_;

        Caller& caller_;
};
