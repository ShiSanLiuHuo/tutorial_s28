#include <iostream>
using namespace std;
class Complex {
private:
    double real;
    double imag;
public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
    Complex operator-(const Complex& c) const {
        return Complex(real - c.real, imag - c.imag);
    }
    void display() const {
        if (imag >= 0) {
            cout << real << " + " << imag << "i" << endl;
        } else {
            cout << real << " - " << -imag << "i" << endl;
        }
    }
};
int main() {
    double r1,r2,i1,i2;
    cin>>r1>>i1>>r2>>i2;
    Complex c1(r1, i1); 
    Complex c2(r2, i2); 
    Complex c3 = c1 - c2;
    cout << "c1 = ";
    c1.display();
    cout << "c2 = ";
    c2.display();
    cout << "c1 - c2 = ";
    c3.display();
    return 0;
}