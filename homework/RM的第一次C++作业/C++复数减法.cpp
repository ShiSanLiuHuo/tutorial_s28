//
//  main.cpp
//  rm的C++作业
//
//  Created by 小坦W on 2026/10/30.
//

#include <cmath>
#include <iostream>

class Complex {
public:
    Complex(double real = 0.0, double imag = 0.0)
        : real_(real), imag_(imag) {}

    Complex operator-(const Complex& other) const {
        return Complex(real_ - other.real_, imag_ - other.imag_);
    }

    friend std::istream& operator>>(std::istream& in, Complex& value) {
        double real;
        double imagMagnitude;
        char sign;
        char unit;

        if (in >> real >> sign >> imagMagnitude >> unit) {
            if ((sign != '+' && sign != '-') || imagMagnitude < 0.0 ||
                (unit != 'i' && unit != 'I')) {
                in.setstate(std::ios::failbit);
            } else {
                const double imag = (sign == '-') ? -imagMagnitude : imagMagnitude;
                value = Complex(real, imag);
            }
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Complex& value) {
        out << value.real_;
        if (value.imag_ >= 0.0) {
            out << " + " << value.imag_ << 'i';
        } else {
            out << " - " << std::abs(value.imag_) << 'i';
        }
        return out;
    }

private:
    double real_;
    double imag_;
};

int main() {
    Complex first;
    Complex second;

    std::cout << "请输入第一个复数：";
    if (!(std::cin >> first)) {
        std::cerr << "输入无效。\n";
        return 1;
    }

    std::cout << "请输入第二个复数：";
    if (!(std::cin >> second)) {
        std::cerr << "输入无效。\n";
        return 1;
    }

    const Complex result = first - second;

    std::cout << "相减结果：" << result << '\n';
    return 0;
}
