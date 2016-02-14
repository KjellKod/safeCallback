#include <functional>
#include <mutex>
#include <unordered_map>
#include <thread>

typedef std::function<void()> Callable;

class Caller{
    public:
        void registerFor(int type, Callable cb);
        void unregister(int type);

        void start();
        void stop();
        void setProbe(Callable beforeRun, Callable afterRun, Callable afterUnReg = Callable());

    private:
        void run();

        std::mutex mutex_;
        std::unordered_map<int, Callable> callbacks_;

        std::thread runner_;
        Callable before_;
        Callable after_;
        Callable afterUnReg_;
};
