#include <iostream>
#include <chrono>
#include <thread.hpp>
#include <thread_safe_iostream.hpp>

void myFunction1() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function1, iteration " << i << std::endl;
    }
}

void myFunction2() {
    for (int i = 0; i < 5; ++i) {
        threadSafeCout << "Hello from Function2, iteration " << i << std::endl;
    }
}

int main() {
    Thread thread1("Thread1", myFunction1);
    Thread thread2("Thread2", myFunction2);

    try
    {
        thread1.stop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    thread1.start();
    try
    {
        thread1.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    thread2.start();

    thread1.stop();
    thread2.stop();


    return 0;
}
