#ifndef POOL_HPP
# define POOL_HPP

# include <iostream>
# include <vector>
# include <functional>

using namespace std;

template<typename TType> // Template that allows your pool to handle any object
class Pool{ 
    public:
        class Object {
            public:
                Object(TType*, function<void(TType*)>);
                ~Object();
                TType* operator->();
            private:
                TType* objPtr;
                function<void(TType*)> deleterFunc;
        };
    void    resize(const size_t& numberOfObjects);
    size_t  size();
    size_t  capacity();

    template<typename... TArgs>
    Object acquire(TArgs&&... args);

    ~Pool(); // Destructor declaration
    private:
        vector<TType*> pool;  // Internal pool storage
        void release(TType* obj);
    
};


template<typename TType>
Pool<TType>::Object::Object(TType* ptr, function<void(TType*)> deleter) :
    objPtr(ptr), deleterFunc(deleter) {}

template<typename TType>
Pool<TType>::Object::~Object()
{
    if (objPtr){
        this->deleterFunc(objPtr);
    }
}

template<typename TType>
TType* Pool<TType>::Object::operator->()
{
    return this->objPtr;
}

template<typename TType>
size_t Pool<TType>::size()
{
    return this->pool.size();
}

template<typename TType>
size_t Pool<TType>::capacity()
{
    return this->pool.capacity();
}

/**
 * Resizes internal container to new numberOfObjects.
 * If size() < numberOfObjects it deletes the allocated objects
 */
template<typename TType>
void Pool<TType>::resize(const size_t& numberOfObjects)
{
    size_t currentSize = pool.size();

    if (numberOfObjects > currentSize) {
        for (size_t i = currentSize; i < numberOfObjects; ++i) {
            TType* newObj = new TType();
            this->pool.push_back(newObj);
        }
    } else if (numberOfObjects < currentSize) {
        for (size_t i = currentSize; i > numberOfObjects; --i) {
            delete this->pool.back();
            this->pool.pop_back();
        }
    }
}

/**
 * Returns object back to pool when deconstructor is called
 */
template<typename TType>
void Pool<TType>::release(TType *obj)
{
    obj->~TType();
    this->pool.push_back(obj);
}

/**
 * Returns a new object from the pool.
 * If there are available objects inside the pool, it returns the 
 * in-memory construction of the object (no new allocation)
 * If there are no available objects, it allocates a new object memory
 */
template<typename TType>
template<typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... args)
{
    if (!this->pool.empty())
    {
        // Gets the last valid element in pool and then placement to use the same memory address
        TType* obj = pool.back();
        this->pool.pop_back();
        new (obj) TType(forward<TArgs>(args)...);
        return  Object(obj, [this](TType *obj) { release(obj); });
    }
    else
    {
        //All objects have been used, so we create a new one
        return Object(new TType(forward<TArgs>(args)...), [this](TType* obj) { release(obj);});
    }
}

/**
 * Deletes all objects in pool
 */
template<typename TType>
Pool<TType>::~Pool()
{
    for (TType* obj : this->pool)
        delete obj;
}

#endif
