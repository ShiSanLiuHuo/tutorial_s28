#include <iostream> 

class Complex { 
private: 
    double real; 
    double imag; 

public: 
     
    Complex(double r = 0, double i = 0) {
        real = r; 
        imag = i;  
    }

   
    Complex operator-(const Complex& other) const {
       
        double new_real = real - other.real; 
        double new_imag = imag - other.imag; 
        
      
        return Complex(new_real, new_imag);
    }

   
    void print() const {
        std::cout << real << " + " << imag << "i" << std::endl;
    }
};


int main() {
    Complex c1(4.0, 5.0); 
    Complex c2(2.0, 3.0); 

    Complex c3 = c1 - c2;  

    c3.print(); 
    return 0; 
}
