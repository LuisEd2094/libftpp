#ifndef SINGLETON_HPP
# define SINGLETON_HPP

# include <stdexcept>
# include <memory>
using namespace std;

template<typename TType>
class Singleton
{
    private:
        Singleton();
        static unique_ptr<TType> p_instance;
    public:
        static TType* instance();

        ~Singleton();
    
        template<typename ... TArgs> 
        static void instanciate(TArgs&&... p_args);
};

template<typename TType>
unique_ptr<TType> Singleton<TType>::p_instance = nullptr;

template<typename TType>
TType* Singleton<TType>::instance()
{
    if (p_instance == nullptr)
        throw runtime_error("Exception: Instance not yet created");
    return p_instance.get();
}

template<typename TType>
template<typename ... TArgs> 
void Singleton<TType>::instanciate(TArgs&&... p_args)
{
    if (p_instance == nullptr)
    {
        p_instance = make_unique<TType>(std::forward<TArgs>(p_args)...);
    }
    else
    {
        throw runtime_error("Exception: Instance already created");
    }
    
}
template<typename TType>
Singleton<TType>::~Singleton(){};


#endif