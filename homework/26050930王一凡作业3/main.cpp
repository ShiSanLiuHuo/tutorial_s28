
#include <iostream>
#include "Matrix.hpp"

void setMatrix(Matrix &matrix, std::initializer_list<double> values)
{
    matrix.assignFrom(values);
}

void printMatrix(const Matrix &matrix)
{
    matrix.print();
}

int main()
{
    Matrix m(2, 2);
    setMatrix(m, { 1, 2, 3, 4 });
    std::cout << "m";
    printMatrix(m);

    Matrix n(2, 2);
    setMatrix(n, { 5, 6, 7, 8 });
    std::cout << "n";
    printMatrix(n);

    std::cout << "m + n" << std::endl;
    printMatrix(m + n);
    std::cout << "m - n" << std::endl;
    printMatrix(m - n);
    std::cout << "m * n" << std::endl;
    printMatrix(m * n);
    std::cout << "m / n" << std::endl;
    printMatrix(m / n);

    Matrix temp = m;
    std::cout << "m" << std::endl;
    printMatrix(temp);
    std::cout << "-m" << std::endl;
    printMatrix(-m);
    std::cout << "m^T" << std::endl;
    printMatrix(m.transpose());
    std::cout << "m^-1" << std::endl;
    printMatrix(m.inverse());
    std::cout << "m == n: " << (m == n) << std::endl;
    std::cout << "m != n: " << (m != n) << std::endl;
    std::cout << "|m|: " << m.determinant() << std::endl;

    return 0;
}
