//accomplishing imaginary number operations by overload
#ifndef IMAGNUM_H_
#define IMAGNUM_H_

class Complex{
public:
    Complex();
    Complex(double imagNum, double realNum);

    std::string show();
    Complex operator+(const Complex& c)const;
    Complex operator-(const Complex& c)const;

private:
    double imagNum_;
    double realNum_;

};

#endif