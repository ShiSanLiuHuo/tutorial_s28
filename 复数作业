#include<iostream>
using namespace std;
class Complex
    {
        public:
        
            Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}           
            Complex operator - (const Complex &c) {
                return Complex(real - c.real, imag - c.imag);
            }
            void print() const {
            cout << real;
            if (imag >= 0) 
                cout << " + " << imag << "i";
            else           
                cout << " - " << -imag << "i";
            }
        private:
            double real, imag;
};
int main() {
    Complex c1(3.0, 4.0);
    Complex c2(1.0, 2.0);
    Complex c3 = c1 - c2;
    cout<<"(";
    c1.print();
    cout<<") - (";
    c2.print();
    cout<<") = ";
    c3.print();
    return 0;
}
