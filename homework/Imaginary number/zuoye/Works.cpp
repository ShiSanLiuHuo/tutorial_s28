#include <iostream>
using namespace std;
class Complex{
    public:
    Complex(double r=0.0 ,double i=0.0):real(r),imag(i){}
    Complex operator - (const Complex &c) const{
    return Complex(real - c.real,imag-c.imag);
    }
    friend ostream &operator<<(ostream &os, const Complex &c) {
    os << c.real << "+" << c.imag << "i";
    return os;
    }
    private:
    double real,imag;
};
int main()
{
    double r1,r2,c1,c2;
    std::cin >> r1 >>c1;
    std::cin >> r2 >>c2;
    Complex a(r1,c1),b(r2,c2);
    Complex Result=a-b;
    cout<<Result<<endl;
    return 0;
}