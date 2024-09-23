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
        std::chrono::time_point<std::chrono::steady_clock> start_time;
    public:
        Thread(const std::string& name, std::function<void()> f);
        ~Thread();
        void start();
        void stop();
        double getElapsedTime() const;
};
Thread::~Thread()
{
    this->stop();
}

Thread::Thread(const std::string& name, std::function<void()> f)
    : name(name), function(f), running(false) {}  

void Thread::start() 
{
    if (!this->running)
    {
        this->running = true;
        this->worker = std::thread([this]() 
        {
            threadSafeCout.setPrefix(name);
            this->start_time = std::chrono::steady_clock::now();
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

double Thread::getElapsedTime() const 
{
    if (this->running) 
    {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now - this->start_time).count();
    }
    return 0.0; // If not running, return 0
}

#endif