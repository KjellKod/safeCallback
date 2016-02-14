#include <functional>
#include <memory>
#include <mutex>
#include <vector>

class Caller;
class Service{
    public:
        Service(Caller& caller);
        Service& operator=(const Service&) = delete;
        ~Service();
        
        void doSomething(std::weak_ptr<int> ref);

        //testing purpose only
        static std::vector<int> probeMarker;
    private:
        std::mutex mutex_;
        std::shared_ptr<int> sync_;
        std::unique_ptr<int> resource_;

        Caller& caller_;
};
