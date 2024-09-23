#ifndef WORKER_POOL_HPP
# define WORKER_POOL_HPP

# include <data_structures.hpp>
# include <observer.hpp>
# include <threading.hpp>
# include <mutex>
# include <condition_variable>


class Worker
{
    friend class WorkerPool;
    private:
        Thread thread;
        Worker();
    public:
        Worker(const std::string& name, std::function<void()> f);
        void start();
        void stop();
        ~Worker();
};

Worker::Worker(const std::string& name, std::function<void()> f): thread(name, f)
{

}

Worker::~Worker()
{
}

void Worker::start()
{
    this->thread.start();
}

void Worker::stop()
{
    this->thread.stop();
}

class WorkerPool
{
    Pool<Worker>                            workers;
    std::mutex                              mutex;
    ThreadSafeQueue<std::function<void()>>  function_queues;
    std::condition_variable                 condition; 
    void work();
    void finish_work();
    std::atomic<bool>                       shouldStop;
    
    WorkerPool();
    public:
        WorkerPool(size_t);
        ~WorkerPool();
        void addJob(const std::function<void()>& f);
        void subscribe();
        
};

void    WorkerPool::finish_work()
{

}

void    WorkerPool::work()
{
    while (1) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            condition.wait(lock, [this]
            {
                return !this->function_queues.empty() || this->shouldStop; 
            }
            );
            if (shouldStop && this->function_queues.empty()) 
            {
                break;  // Exit the loop if we're stopping and no jobs are left
            }
            if (!this->function_queues.empty())
                job = std::move(this->function_queues.pop_front());
        }
        if (job) {
            job();
            job = nullptr;
        }
    }
}


WorkerPool::WorkerPool(size_t workers) : workers(workers, "", std::bind(&WorkerPool::work, this)), shouldStop(false)
{
    for (auto& worker : this->workers)
    {
        worker->start();
    }

}

WorkerPool::~WorkerPool()
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        shouldStop = true; // Signal workers to stop
    }
    condition.notify_all(); // Wake up all waiting workers
    // Wait for all workers to finish

}


void WorkerPool::addJob(const std::function<void()>& f)
{
    // If we still have workers that have not being started, assign them their work
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        this->function_queues.push_back(f);
    }
    this->condition.notify_one();
}


#endif