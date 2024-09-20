class A{
    int _x;
    public:
        A(int x) : _x(x){}
        ~A(){ }
        int getX() {return (_x);}
};

#include <iostream>
#include <vector>

using namespace std;
int main()
{
    A* a = new A(5);
    std::cout << a->getX() << std::endl;

    a->~A();
    a = new (a)A(10);  // Placement new
    std::cout << a->getX() << std::endl;

    delete a;

    vector<int> vec;
    std::cout << "Capacity " << vec.capacity() << std::endl;
    std::cout << vec.size() << std::endl;

    vec.push_back(1);

    std::cout << vec.capacity() << std::endl;
    std::cout << vec.size() << std::endl;
    std::cout << vec.back() << std::endl;
    vec.pop_back();
    std::cout << vec.capacity() << std::endl;
    std::cout << vec.size() << std::endl;
    std::cout << vec.back() << std::endl;

}