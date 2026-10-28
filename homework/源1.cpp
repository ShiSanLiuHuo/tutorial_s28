#include<iostream>
using namespace std;
struct complex
{
	float real;
	float imag;
};
complex operator -(complex a, complex b)
{
	complex c;
	c.real = a.real - b.real;
	c.imag = a.imag - b.imag;
	return c;
}
int main()
{
	complex a,b;
	cout << "请输入第一个复数的实部和虚部："<<endl;
	cin >> a.real >> a.imag;
	cout << "请输入第二个复数的实部和虚部："<<endl;
	cin >> b.real >> b.imag;
	complex c = a - b;
	cout << "两个复数的差为：" << c.real << (c.imag >= 0 ? "+" : "") << c.imag << "i" << endl;
	return 0;

}
