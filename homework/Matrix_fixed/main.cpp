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

#include <iostream>
#include <utility>
#include "Matrix.hpp"
// hamocat新增参数numValues，毕竟如果setMatrix需要用退化成指针的数组，就无从得知数组的元素个数了
// 但即使是这么处理，这样的实现还是很拙劣的，毕竟无法保证输入的numValues就是数组的元素个数。
void setMatrix(Matrix &matrix, double* values, int numValues){
    // set 函数
    // matrix.set(1, 1, values[0]).set(1, 2, values[1])
    //       .set(2, 1, values[2]).set(2, 2, values[3]);
    // 或者自己写遍历（本实现下标从 0 开始）：
    // int rows = matrix.getRows();
    // int cols = matrix.getCols();
    // int k = 0;
    // for (int i = 0; i < rows && k < numValues; i ++){
    //     for (int j = 0; j < cols && k < numValues; j ++){
    //             matrix.set(i, j, values[k]);
    //             k++;
    //     }
    // }

    // 重载运算符 <<，实现矩阵的输入
    // 坑：不能照上面的注释写成 matrix << values[0], values[1], values[2], values[3];
    //     逗号运算符的优先级比 << 低，整句会被解析成 (((matrix << values[0]), values[1]), ...)，
    //     每遇到一个逗号，前一段的结果就被丢弃，所以实际只有 values[0] 进了矩阵。
    //     要链式输入必须写成 matrix << values[0] << values[1] << ...
    for (int k = 0; k < numValues; k ++){
        matrix << values[k];
    }
}

void printMatrix(const Matrix &matrix){
    // print 函数
    // matrix.print();

    // 重载流运算符 <<，实现矩阵的输出
    std::cout << matrix;

    // 重载运算符 ()，实现矩阵的访问（本实现下标从 0 开始）
    // std::cout << matrix(0, 0) << matrix(0, 1) << std::endl
    //           << matrix(1, 0) << matrix(1, 1) << std::endl;
}

int main(){
    Matrix m(2, 2);
    double values1[] = { 1, 2, 3, 4 };
    setMatrix(m, values1, 4);
    std::cout << "m" << std::endl;
    printMatrix(m);

    Matrix n(2, 2);
    double values2[] = { 5, 6, 7, 8 };
    setMatrix(n, values2, 4);
    std::cout << "n" << std::endl;
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

    // 重载运算符 ()，实现矩阵的访问
    Matrix t(2, 2);
    t << 1 << 2 << 3 << 4;
    std::cout << "t(0, 0): " << t(0, 0) << std::endl;
    // 非 const 版本返回的是引用，所以可以直接赋值
    t(0, 0) = 9.5;
    std::cout << "t(0, 0) = 9.5 之后" << std::endl;
    printMatrix(t);
    // 越界访问只会打印错误提示，不会崩溃
    std::cout << "t(3, 3): " << t(3, 3) << std::endl;

    // 重载运算符 =，实现拷贝赋值和移动赋值
    Matrix copy;
    copy = t;
    std::cout << "copy = t" << std::endl;
    printMatrix(copy);

    Matrix moved;
    moved = std::move(copy);
    std::cout << "moved = std::move(copy) 之后 copy 的行列数: "
              << copy.getRows() << " " << copy.getCols() << std::endl;
    printMatrix(moved);

    // 3 阶矩阵的行列式和逆矩阵，用来验证 bomb() 的递归展开
    Matrix a(3, 3);
    a << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 10;
    std::cout << "|a|: " << a.determinant() << std::endl;
    std::cout << "a^-1" << std::endl;
    printMatrix(a.inverse());
    std::cout << "a * a^-1" << std::endl;
    printMatrix(a * a.inverse());

    return 0;
}
