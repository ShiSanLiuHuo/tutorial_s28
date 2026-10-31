#include<iostream>
using namespace std;

class Complex
{
    public:
    double real;
    double imag;
    Complex(double r=0,double i=0){
        real=r;
        imag=i;
    }
    Complex operator-(const Complex&other){
        return Complex(real-other.real,imag-other.imag);
    }
    void res()
    {
        cout<<real;
        if(imag>0){
            cout<<"+"<<imag<<"i";
        }
        else if(imag<0){
            cout<<imag<<"i";
        }
        cout<<endl;
    }
};

int main(void)
{
    Complex z1(7,7);
    Complex z2(3,5);
    Complex z3=z1-z2;
    z3.res();
    return 0;
}