#ifndef DATA_BUFFER_HPP
# define DATA_BUFFER_HPP

#include <type_traits>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstring>

using namespace std;


class DataBuffer
{
    public:
        // << OPERATORS

        template<typename T>
        DataBuffer& operator<<(const T& obj);    
        DataBuffer& operator<<(const string& str);        
        template <typename T>
        DataBuffer& operator<<(const std::vector<T>& vec);

        // >> OPERATORS 

        template<typename T>
        DataBuffer& operator>>(T& obj);
        DataBuffer& operator>>(string& str);
        template<typename T>
        DataBuffer& operator>>(vector<T>& vec);
    private:
        std::vector<uint8_t> buffer;

};

// << OPERATORS
template<typename T>
DataBuffer& DataBuffer::operator<<(const T& obj)
{
    static_assert(is_trivially_copyable<T>::value, "Object must be trivially copyable");

    const uint8_t * dataPtr = reinterpret_cast<const uint8_t *>(&obj);
    buffer.insert(buffer.end(), dataPtr, dataPtr + sizeof(T));
    return *this;
}        
DataBuffer& DataBuffer::operator<<(const string& str)
{
    size_t length = str.size();
    *this << length; //Insert length so we know how big the thing is

    buffer.insert(buffer.end(), str.begin(), str.end());
    return *this;
}
template <typename T>
DataBuffer& DataBuffer::operator<<(const std::vector<T>& vec) 
{
    size_t size = vec.size();
    *this << size; 
    for (const auto& elem : vec) {
        *this << elem;
    }
    return *this;
}

// >> OPERATORS 
template<typename T>
DataBuffer& DataBuffer::operator>>(T& obj) 
{
    static_assert(std::is_trivially_copyable<T>::value, "Object must be trivially copyable.");

    if (buffer.size() < sizeof(T)) {
        throw std::runtime_error("Not enough data in buffer");
    }
    std::memcpy(&obj, buffer.data(), sizeof(T));
    buffer.erase(buffer.begin(), buffer.begin() + sizeof(T));
    return *this;
}
DataBuffer& DataBuffer::operator>>(string& str)
{
    size_t length;
    *this >> length;
    if (buffer.size() < length) 
    {
        throw std::runtime_error("Not enough data in buffer");
    }
    str.assign(buffer.begin(), buffer.begin() +  length);
    buffer.erase(buffer.begin(), buffer.begin() + length);
    return *this;
}
template<typename T>
DataBuffer& DataBuffer::operator>>(vector<T>& vec) 
{
    size_t size;
    *this >> size;

    if (buffer.size() < size) 
    {
        throw std::runtime_error("Not enough data in buffer");
    }
    vec.resize(size);
    for (size_t i = 0; i < size; ++i) 
    {
        *this >> vec[i];
    }
    return *this;
}


// Example for more complex operations
class A
{
    public:
        int a;
        A(int p_a) : a(p_a){};
        A(): a(42){};

        friend DataBuffer& operator<<(DataBuffer& p_buffer, const A& p_object) {
            p_buffer << p_object.a;
            return p_buffer;
        }
        friend DataBuffer& operator>>(DataBuffer& p_buffer, A& p_object) {
            p_buffer >> p_object.a;
            return p_buffer;
        }
};

class ComplexObject
{
    private:
        vector<string>vec;
        A a; 

    public:
        ComplexObject() : vec(vector<string>(50, "Hola")) {}
        ComplexObject(const vector<string> p_vec) : vec(p_vec), a((int)p_vec[0].size()) {}

        friend DataBuffer& operator<<(DataBuffer& p_buffer, const ComplexObject& p_object) {
            p_buffer << p_object.vec << p_object.a;
            return p_buffer;
        }
        friend DataBuffer& operator>>(DataBuffer& p_buffer, ComplexObject& p_object) {
            p_buffer >> p_object.vec >> p_object.a;
            return p_buffer;
        }
};

#endif