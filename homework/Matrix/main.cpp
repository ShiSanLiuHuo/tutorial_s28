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

// 具体功能实现
// author: hamocat (github: GaryGa)
// 关注永雏塔菲谢谢喵
// 2026.11.22

#include "Matrix.hpp"
#include <iostream>
// hamocat新增参数numValues，毕竟如果setMatrix需要用退化成指针的数组，就无从得知数组的元素个数了
// 但即使是这么处理，这样的实现还是很拙劣的，毕竟无法保证输入的numValues就是数组的元素个数。
void setMatrix(Matrix &matrix, double *values, int numValues) {
    int rows = matrix.getRows();
    int cols = matrix.getCols();
    int k = 0;

    // set 函数
    // matrix.set(1, 1, values[0]).set(1, 2, values[1])
    //       .set(2, 1, values[2]).set(2, 2, values[3]);
    for (int i = 0; i < rows && k < numValues; i++) {
        for (int j = 0; j < cols && k < numValues; j++) {
            matrix.set(i, j, values[k]);
            k++;
        }
    }

    // 重载运算符 <<，实现矩阵的输入
    // matrix << values[0], values[1], values[2], values[3];
    // hamocat: 这个版本先不实现哈，拜拜捏
}

// void printMatrix(const Matrix &matrix) {
    // print 函数
    // matrix.print();

    // 重载流运算符 <<，实现矩阵的输出
    // std::cout << matrix;

    // 重载运算符 ()，实现矩阵的访问
    // std::cout << matrix(1, 1) << matrix(1, 2) << std::endl
    //           << matrix(2, 1) << matrix(2, 2) << std::endl;
// }

int main() {
    Matrix m(2, 2);
    double values1[] = {1, 2, 3, 4};
    setMatrix(m, values1, 4);
    std::cout << "this is matrix m, here prints it out: " << std::endl << m
              << "and here prints its transpose: " << std::endl << m.transpose()
              << "and here is its determinant, |m| = " << m.determinant() << std::endl
              << "and here prints its inverse: " << std::endl << m.inverse();
    // printMatrix(m);

    Matrix n(2, 2);
    double values2[] = {3, 6, 9, 12};
    setMatrix(n, values2, 4);
    std::cout << "this is matrix n, here prints it out: " << std::endl << n
              << "we have m + n: " << std::endl << m + n
              << "and m - n:" << std::endl << m - n
              << "and 3 times m: " << std::endl << m * 3
              << "and n / 3" << std::endl << n / 3
              << "and dot product of m * n: " << std:: endl << m * n;

//     std::cout << "n";
//     printMatrix(n);
//
//     std::cout << "m + n" << std::endl;
//     printMatrix(m + n);
//
//     std::cout << "m - n" << std::endl;
//     printMatrix(m - n);
//
//     std::cout << "m * n" << std::endl;
//     printMatrix(m * n);
//
//     std::cout << "m / n" << std::endl;
//     printMatrix(m / n);
//
//     Matrix temp = m;
//     std::cout << "m" << std::endl;
//     printMatrix(temp);
//
//     std::cout << "-m" << std::endl;
//     printMatrix(-m);
//
//     std::cout << "m^T" << std::endl;
//     printMatrix(m.transpose());
//
//     std::cout << "m^-1" << std::endl;
//     printMatrix(m.inverse());
//
//     std::cout << "m == n: " << (m == n) << std::endl;
//     std::cout << "m != n: " << (m != n) << std::endl;
//
//     std::cout << "|m|: " << m.determinant() << std::endl;

    return 0;
}
