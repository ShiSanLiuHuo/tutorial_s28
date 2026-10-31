#include <iostream>

class Complex {
public:
    Complex(double real = 0.0, double imag = 0.0) : real_(real), imag_(imag) {}

    // 重载减法运算符：返回两个复数相减后的新对象
    Complex operator-(const Complex& other) const {
        return Complex(real_ - other.real_, imag_ - other.imag_);
    }

    // 重载输出运算符，便于直接打印复数
    friend std::ostream& operator<<(std::ostream& os, const Complex& value) {
        os << value.real_;
        if (value.imag_ >= 0.0) {
            os << " + " << value.imag_ << "i";
        } else {
            os << " - " << -value.imag_ << "i";
        }
        return os;
    }

private:
    double real_;  // 实部
    double imag_;  // 虚部
};

int main() {
    const Complex z1(5.0, 3.0);   // 5 + 3i
    const Complex z2(2.0, 1.0);   // 2 + 1i
    const Complex z3(1.0, 2.0);   // 1 + 2i
    const Complex z4(3.0, 5.0);   // 3 + 5i
    const Complex z5(4.0, -2.0);  // 4 - 2i

    std::cout << "z1 = " << z1 << '\n';
    std::cout << "z2 = " << z2 << "\n\n";

    std::cout << "z1 - z2 = " << (z1 - z2) << '\n';
    std::cout << "z2 - z1 = " << (z2 - z1) << '\n';
    std::cout << "z3 - z4 = " << (z3 - z4) << '\n';
    std::cout << "z5 - z5 = " << (z5 - z5) << '\n';

    return 0;
}