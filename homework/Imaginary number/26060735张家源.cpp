#include <iostream> // 引⼊流输⼊输出库
using namespace std;//命名空间
    class Complex {
public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
    //定义了⼀个包含虚部和实部的数据类型
    Complex operator - (const Complex &c) {
    //数据类型 operator 重载的运算符（这⾥是-，c++中⼤部分运算符都可以进⾏重载）
    //括号⾥的内容:
    //const:承诺函数内部不修改 c。既保护实参，⼜让函数能接收 const 对象和临时对象；
    //不加的话，const Complex x; x - y 会编译失败
    //&是⽤引引用的⽅式传参数，不然会复制整⼀个参数，效率太低且可以直接操作原对象
    return Complex(real - c.real, imag - c.imag);
    //返回值就是虚部相减和实部相减
}
    void print() const{
        cout << "(" << real << ", " << imag << ")" << endl;
    }
private:
    double real, imag;
};
int main() {
    Complex c1(1.0, 2.0), c2(3.0, 4.0);
    Complex c3 = c1 - c2;
    //cout << "c3 = (" << c3.real << ", " << c3.imag << ")" << endl;
    cout << "c3 = ";
    c3.print();
    return 0;
}
