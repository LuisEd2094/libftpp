class A{
    int _x;
    public:
        A(int x) : _x(x){}
        ~A(){ }
        int getX() {return (_x);}
};

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;
int main()
{
    std::ostringstream buffer;

    buffer << "HOLA" << std::endl; 
    std::cout << "✋" << std::endl;
}