#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

class Matrix {
public:
    // 默认构造函数（hamocat新增）
    Matrix();
    // 构造函数
    Matrix(int rows, int cols);
    // 拷贝构造函数
    Matrix(const Matrix &other);
    // 移动构造函数，思考和拷贝构造函数的区别（hamocat新增noexcept）
    Matrix(Matrix &&other)noexcept;
    // 析构函数
    ~Matrix();

    // 设置矩阵元素
    Matrix &set(int row, int col, double value);
    // 获取矩阵元素
    double get(int row, int col) const;

    // 获取矩阵行
    double *getRow(int row) const;
    // 设置矩阵行（hamocat新增numValues，原因见main.cpp）
    Matrix &setRow(int row, double *values, int numValues);

    // 获取矩阵列
    double *getCol(int col) const;
    // 设置矩阵列（hamocat新增numValues，原因同理见main.cpp）
    Matrix &setCol(int col, double *values, int numValues);

    // 获取矩阵行数
    int getRows() const;
    // 设置矩阵行数（hamocat新增）
    void setRows(int rows);

    // 获取矩阵列数
    int getCols() const;
    // 设置矩阵列数（hamocat新增）
    void setCols(int cols);

    // 获取矩阵数据
    double **getData() const;
    // 设置矩阵数据（hamocat新增）
    void setData(double **data);

    // 打印矩阵（hamocat修改返回值为void，鉴于打印矩阵不需要返回原矩阵）
    void print() const;

    // 矩阵加法
    Matrix add(const Matrix &other) const;
    // 矩阵乘法
    Matrix multiply(const Matrix &other) const;
    // 矩阵转置
    Matrix transpose() const;
    // 矩阵是否为方阵
    bool isSquare() const;
    // bomb!!!（hamocat）
    Matrix bomb(int row, int col)const;
    // 矩阵行列式
    double determinant() const;
    // 矩阵求逆
    Matrix inverse() const;

    // 重载运算符 + - * / = == !=, 实现矩阵的加减乘除赋值比较
    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator-() const;
    Matrix operator*(const Matrix &other) const;
    Matrix operator/(const Matrix &other) const;
    Matrix &operator=(const Matrix &other); // 拷贝赋值
    Matrix &operator=(Matrix &&other); // 移动赋值，思考和拷贝赋值的区别
    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;
    // 重载运算符 ()，实现矩阵的访问
    // （hamocat修改：原模板返回int，会把double元素截断成整数，这里改成double / double&）
    double operator()(int row, int col) const;
    double &operator()(int row, int col);
    // 重载运算符 <<，实现矩阵的输入，如：Matrix m(2, 2); m << 1 << 2 << 3 << 4;
    Matrix &operator<<(double value);

    // 重载流运算符 <<，实现矩阵的输出
    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

private:
    int rows_; // 矩阵行数
    int cols_; // 矩阵列数
    double** data_; // 矩阵数据
    int cursor_; // 重载运算符<<输入时下一个待写入的位置，按行优先顺序移动（hamocat新增）
};

#endif // MATRIX_HPP
