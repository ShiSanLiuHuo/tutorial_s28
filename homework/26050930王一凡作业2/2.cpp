#include <iostream>
using namespace std;

class Complex {
private:
    double real;
    double imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    Complex operator-(const Complex &c) const {
        return Complex(real - c.real, imag - c.imag);
    }

    void show() const {
        cout << real;
        if (imag >= 0) cout << "+" << imag << "i";
        else cout << imag << "i";
        cout << endl;
    }
};

int main() {
    Complex c1(5.0, 4.0);
    Complex c2(2.0, 1.5);
    Complex c3 = c1 - c2;

    cout << "c1 = "; c1.show();
    cout << "c2 = "; c2.show();
    cout << "c1 - c2 = "; c3.show();

    return 0;
}
