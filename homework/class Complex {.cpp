class Complex {
    public:
        Complex(double r=0.0, double i=0.0) : real(r), imag(i) {}
        Complex operator-(const Complex& other) const {
            return Complex(real - other.real, imag - other.imag);
        }
private:
    double real, imag;
}