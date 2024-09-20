#include <iostream>
#include <vector>
#include <memory>
#include <functional>

template<typename TType>
class Pool {
public:
    // Wrapper for pool object that behaves like a smart pointer
    class Object {
    public:
        Object(TType* ptr, std::function<void(TType*)> deleter)
            : objPtr(ptr), deleterFunc(deleter) { }

        // Destructor automatically returns the object to the pool when it goes out of scope
        ~Object() {
            if (objPtr) {
                deleterFunc(objPtr);
            }
        }

        // Overload operator-> to provide access to the object's members
        TType* operator->() {
            return objPtr;
        }

    private:
        TType* objPtr;
        std::function<void(TType*)> deleterFunc;  // Custom deleter to return the object to the pool
    };

    // Resize the pool and preallocate objects
    void resize(const size_t& numberOfObjects) {
        for (size_t i = pool.size(); i < numberOfObjects; ++i) {
            pool.push_back(new TType());
        }
    }

    // Acquire an object from the pool, using constructor arguments if necessary
    template<typename... TArgs>
    Object acquire(TArgs&&... args) {
        if (!pool.empty()) {
            // Reuse an existing object from the pool
            TType* obj = pool.back();
            pool.pop_back();
            new (obj) TType(std::forward<TArgs>(args)...);  // Reconstruct the object in-place
            return Object(obj, [this](TType* obj) { release(obj); });
        } else {
            // Create a new object if no pre-allocated ones are available
            return Object(new TType(std::forward<TArgs>(args)...), [this](TType* obj) { release(obj); });
        }
    }

private:
    // Return an object to the pool
    void release(TType* obj) {
        obj->~TType();  // Call destructor explicitly to clean up the object
        pool.push_back(obj);  // Return the object to the pool
    }

    std::vector<TType*> pool;  // Internal pool storage
};

// Example class to be used with the pool
class TestObject {
public:
    TestObject(int x = 0) : _x(x) {
        std::cout << "TestObject constructed with value: " << _x << std::endl;
    }

    ~TestObject() {
        std::cout << "TestObject destroyed" << std::endl;
    }

    void sayHello() {
        std::cout << "Hello from TestObject with value: " << _x << std::endl;
    }

    void reset(int x) {
        _x = x;
    }

private:
    int _x;
};

int main() {
    Pool<TestObject> myPool;

    // Pre-allocate space for 3 objects in the pool
    myPool.resize(3);

    {
        // Acquire an object with the argument 15, reusing a pre-allocated one
        auto obj1 = myPool.acquire(15);
        obj1->sayHello();  // Output: "Hello from TestObject with value: 15"
    }  // obj1 is released back to the pool here

    {
        // Acquire another object in a different scope
        auto obj2 = myPool.acquire(10);
        obj2->sayHello();  // Output: "Hello from TestObject with value: 10"
    }  // obj2 is released back to the pool here

    {
        // Acquire yet another object from the pool
        auto obj3 = myPool.acquire(20);
        obj3->sayHello();  // Output: "Hello from TestObject with value: 20"
    }  // obj3 is released back to the pool here

    return 0;
}
