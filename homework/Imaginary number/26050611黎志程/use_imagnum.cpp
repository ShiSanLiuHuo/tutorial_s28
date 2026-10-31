#include <iostream>
#include "imagnum.h"

int main(){
    Complex c1(4.0, 2.0), c2(-4.0, 3.0);
    Complex c3 = c1 + c2, c4 = c1 - c2;

    std::cout << "the sum of " << c1.show() << " and " << c2.show() << ": " << c3.show() << std::endl;

    std::cout << "the difference of " << c1.show() << " and " << c2.show() << ": " << c4.show() << std::endl;    

    return 0;
}