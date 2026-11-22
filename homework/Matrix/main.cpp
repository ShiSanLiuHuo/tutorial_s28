// C++ 作业，矩阵类的实现
// author: Haoyu Chen @ HDU-PHOENIX
// 2024.11.30
//
// 1. 完善矩阵类的实现，每正确实现一个函数 3 分，共 99 分；
// 2. 函数实现在 Matrix.cpp 文件中，0.5 分；
// 3. 正确编写 CMakelists.txt 文件，0.5 分。
//
// 附加题：对于每个函数，是否进行了异常处理，共 20 分，单独计分。
//
// 无法实现的功能，注释掉声明以及 main.cpp 中的调用。
// setMatrix() 函数用于设置矩阵，任选一种方式即可；
//              若都未实现，请自行添补遍历赋值。
// printMatrix() 函数用于输出矩阵，任选一种方式即可；
//               若都未实现，请自行添补遍历输出。
// main() 中的矩阵大小和数据均可修改。
// main.cpp 和 Matrix.hpp，除以上情况外不可修改。
//
// ps: 不要骗分哦～

#include <iostream>

#include "Matrix.hpp"

void setMatrix(Matrix &matrix, double *values)
{
    matrix.set(1,1,values[0])
          .set(1,2,values[1])
          .set(2,1,values[2])
          .set(2,2,values[3]);
    // 重载运算符 <<，实现矩阵的输入
    matrix << values[0] << values[1] << values[2] << values[3] << values[4]; // matrix << values[0], values[1], values[2], values[3];
}

void printMatrix(const Matrix &matrix)
{
    // print 函数
    matrix.print();

    // 重载流运算符 <<，实现矩阵的输出
    std::cout <<matrix;// std::cout << matrix;

    // 重载运算符 ()，实现矩阵的访问
    std::cout << matrix(1,1) << matrix(1,2) << std::endl;// std::cout << matrix(1, 1) << matrix(1, 2) << std::endl
    std::cout << matrix(2,1) << matrix(2,2) << std::endl;//           << matrix(2, 1) << matrix(2, 2) << std::endl;
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