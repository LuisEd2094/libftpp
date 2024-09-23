#ifndef THREAD_SAFE_QUEUE_HPP
# define THREAD_SAFE_QUEUE_HPP

# include <deque>
# include <mutex>
# include <stdexcept>

using namespace std;

template<typename TType>
class ThreadSafeQueue
{
    deque<TType>    thread_queue;
    std::mutex      mutex;

    public:
        void push_back(const TType& newElement);
        void push_front(const TType& newElement);
        TType pop_back();
        TType pop_front();
};

template<typename TType>
void ThreadSafeQueue<TType>::push_back(const TType& newElement)
{
    std::lock_guard<std::mutex> lock(this->mutex);
    this->thread_queue.push_back(newElement); 
}

template<typename TType>
void ThreadSafeQueue<TType>::push_front(const TType& newElement)
{
    std::lock_guard<std::mutex> lock(this->mutex);
    this->thread_queue.push_front(newElement); 
}

template<typename TType>
TType ThreadSafeQueue<TType>::pop_back()
{
    std::lock_guard<std::mutex> lock(this->mutex);
    if (this->thread_queue.empty())
        throw runtime_error("Queue is empty");
    TType temp = this->thread_queue.back();
    this->thread_queue.pop_back();
    return temp;  
}

template<typename TType>
TType ThreadSafeQueue<TType>::pop_front()
{
    std::lock_guard<std::mutex> lock(this->mutex);
    if (this->thread_queue.empty())
        throw runtime_error("Queue is empty");
    TType temp = this->thread_queue.front();
    this->thread_queue.pop_front();
    return temp;  
}



#endif