#ifndef THREAD_HPP
# define THREAD_HPP

# include <functional>
# include <atomic>
# include <thread>
# include <thread_safe_iostream.hpp>
# include <stdexcept>


using namespace std;

class Thread {
    private:
        std::string name;
        std::function<void()> function;
        std::thread worker;
        std::atomic<bool> running;
    public:
        Thread(const std::string& name, std::function<void()> f);
        void start();
        void stop();

};

Thread::Thread(const std::string& name, std::function<void()> f)
    : name(name), function(f), running(false) {}  

void Thread::start() {
    if (!this->running)
    {
        this->running = true;
        this->worker = std::thread([this]() 
        {
            threadSafeCout.setPrefix(name);
            this->function();
        });
    }
    else
    {
        throw runtime_error("Thread is currently running");

    }
}  

void Thread::stop() {
    if (this->running) 
    {
        this->running = false;
        if (this->worker.joinable()) 
        {
            this->worker.join();
        }
    }
    else
    {
        throw runtime_error("Thread is not currently running");
    }
}

#endif