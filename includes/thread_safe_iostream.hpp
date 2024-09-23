#ifndef THREAD_SAFE_IOSTREAN_HPP
# define THREAD_SAFE_IOSTREAN_HPP

# include <iostream>
# include <mutex>
# include <data_buffer.hpp>
# include <sstream>
# include <thread>



using namespace std;

class ThreadSafeIOStream
{

private:
    std::ostream&   outputStream;
    std::istream&   inputStream;
    std::mutex      mutex;
    thread_local static std::string threadLocalPrefix;
    thread_local static bool     is_empty;


    static std::stringstream& threadLocalBuffer();
    
    public:
        ThreadSafeIOStream(std::ostream&, std::istream&);
        void setPrefix(const std::string&);
        template <typename T>
        ThreadSafeIOStream& operator<<(const T&);
        ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&)); // for functions line endl
        
        template <typename T>
        ThreadSafeIOStream& operator>>(T& data);



};

ThreadSafeIOStream::ThreadSafeIOStream(std::ostream& outStream = std::cout, std::istream& inStream = std::cin) : 
        outputStream(outStream), inputStream(inStream) {}

void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
    this->threadLocalPrefix = prefix;
}

template <typename T>
ThreadSafeIOStream&  ThreadSafeIOStream::operator<<(const T& data) 
{
    // Buffer the data into the string stream
    threadLocalBuffer() << data;
    is_empty = false;
    return *this;
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&)) 
{
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl) ||
        manip == static_cast<std::ostream& (*)(std::ostream&)>(std::flush)) 
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
        {
            threadLocalBuffer() << '\n';
            outputStream << threadLocalBuffer().str();
        }
        else
        {
            outputStream << threadLocalBuffer().str() << manip;
        }
        threadLocalBuffer().str("");  // Clear the buffer after output
        is_empty = true;
    }

    return *this;
}

template <typename T>
ThreadSafeIOStream& ThreadSafeIOStream::operator>>(T& data) {
    std::lock_guard<std::mutex> lock(this->mutex);  // Ensure thread-safe input

    // Read input into the provided data
    this->inputStream >> data;

    return *this;
}

std::stringstream& ThreadSafeIOStream::threadLocalBuffer() 
{
    static thread_local std::stringstream buffer;
    if (!threadLocalPrefix.empty() && is_empty)
        buffer << threadLocalPrefix;
    return buffer;
}

thread_local ThreadSafeIOStream threadSafeCout(std::cout, std::cin);
thread_local std::string ThreadSafeIOStream::threadLocalPrefix = "";
thread_local bool  ThreadSafeIOStream::is_empty = true;

#endif