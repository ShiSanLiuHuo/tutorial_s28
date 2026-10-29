class Complex {
public:
Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
Complex operator - (const Complex &c) {
return Complex(real - c.real, imag - c.imag);
}
private:
double real, imag;
};