#include <iostream>
#include "imagnum.h"

using namespace std;

Complex::Complex(){
    imagNum_ = 0;
    realNum_ = 0;
}

Complex::Complex(double imagNum, double realNum):imagNum_(imagNum), realNum_(realNum){}

string Complex::show(){
    cout << realNum_ << " + " << imagNum_ << "i";

    return "";
}

Complex Complex::operator+(const Complex& c)const{
    double tempImag, tempReal;
    tempImag = c.imagNum_ + imagNum_;
    tempReal = c.realNum_ + realNum_;
    Complex tempComp(tempImag, tempReal);
    
    return tempComp;
}

Complex Complex::operator-(const Complex& c)const{
    double tempImag, tempReal;
    tempImag = c.imagNum_ - imagNum_;
    tempReal = c.realNum_ - realNum_;
    Complex tempComp(tempImag, tempReal);
    
    return tempComp;
}