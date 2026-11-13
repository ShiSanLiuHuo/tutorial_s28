#include <iostream>

class Complex{
    
public:
    Complex(double r = 0.0, double i = 0.0):real(r),imag(i){}

    Complex operator -(const Complex &c) const {
        return Complex(real - c.real, imag - c.imag);

    }
    void showcomplex() const {
        std::cout << real << " + " << imag << "i" << '\n';
    }

private:
    double real,imag;
};

int main(){
    double r1,i1,r2,i2;
    std::cout << "请输入第一个虚数的实部和虚部: ";
    std::cin >> r1 >> i1;
    std::cout << "请输入第二个虚数的实部和虚部: ";
    std::cin >> r2 >> i2;
    Complex complex1(r1,i1);
    Complex complex2(r2,i2);
    Complex c3 = complex1 - complex2;
    std::cout << "两个虚数的差为: ";
    c3.showcomplex();
    
    return 0;
}