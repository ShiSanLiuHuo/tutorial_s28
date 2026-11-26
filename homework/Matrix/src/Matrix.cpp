// C++ 作业，矩阵类的实现
// author: hamocat (github: GaryGa)
// 关注永雏塔菲谢谢喵
// 鉴于给定的函数接口，矩阵结构通过最简单的二维数组实现，析构也用了手动delete，
// 很遗憾没有用到vector和智能指针等c++特性。
// 希望能抽出时间重写第二版，但这样的话原来很多接口就要修改了。
// 2026.11.22
//
// 第一次修改：
// 修复了构造和析构时未正常new和delete的问题。
// 修复了getRow(),getCol()未正常delete的问题。
// 修复了求矩阵逆、矩阵点积的错误。
// 注：区分函数功能对内还是对外：外部用于获取信息的函数不应参与内部数据处理
// 2026.11.23
//
// 第二次修改：
// 修复了求行列式从1基开始的错误
// 修复了求伴随矩阵代数余子式正负值实现的错误
// 修复了求矩阵转置，用其他方法实现了转置函数的功能
// 新增了输出时一定程度上保证数字对齐，以及边框，但小数点精度需要手动设置
// deepseek检测出有内存泄露问题，需要修改
// 1.检测内存泄露的调试工具（弄清编译选项含义）
// 2.怎么在写代码的时候避免（用更安全的工具和实现； 维护性，错误处理）
// 2026.11.24
//
// 第三次修改：
// 修改了移动赋值和拷贝赋值内部实现，并将返回值改成引用传递，否则按值传递消耗资源
// 修复了移动赋值的内存泄漏
// 只剩下 << 流式输出未实现，原矩阵类的私有成员不足够完成这个功能，故略去。
// 2026.11.26
#include "Matrix.hpp"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>

// 默认构造函数
Matrix::Matrix() {
    rows_ = 0;
    cols_ = 0;
    data_ = nullptr;
}

// 构造函数
// new需要做未正常分配内存的错误处理吗？
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
    data_ = new double *[rows_];
    for (int i = 0; i < rows_; i++) {
        *(data_ + i) = new double[cols]();
    }
}

// 拷贝构造函数
Matrix::Matrix(const Matrix &other)
    : rows_(other.getRows()), cols_(other.getCols()) {
    data_ = new double *[rows_];
    for (int i = 0; i < rows_; i++) {
        *(data_ + i) = new double[cols_];
        for (int j = 0; j < cols_; j++) {
            data_[i][j] = other.get(i, j);
        }
    }
}

// 移动构造函数
Matrix::Matrix(Matrix &&other) noexcept
    : rows_(other.getRows()), cols_(other.getCols()), data_(other.getData()) {
    other.setRows(0);
    other.setCols(0);
    other.setData(nullptr);
}

// 析构函数
Matrix::~Matrix() {
    if (data_) {
        for (int i = 0; i < rows_; i++) {
            delete[] *(data_ + i);
        }
        delete[] data_;
    }
}

// 外部调用：(链式)设置矩阵元素
Matrix &Matrix::set(int row, int col, double value) {
    data_[row][col] = value;
    return *this;
}

// 外部调用：获取矩阵元素
double Matrix::get(int row, int col) const {
    double value = data_[row][col];
    return value;
}

// 外部调用：获取矩阵行
// 第一次修改：不应该直接返回data_[row]，否则能通过getRow直接修改矩阵行，违背了设计的初衷。
// 使用完需要delete!!!!!!!!!!!
double *Matrix::getRow(int row) const {
    double *specificRow = new double[cols_]();
    for (int i = 0; i < cols_; i++) {
        specificRow[i] = data_[row][i];
    }
    return specificRow;
}

// 外部调用：设置矩阵行（hamocat新增numValues，原因见main.cpp）
// 注：结果不会判断用来设置矩阵行的数组是否过短、截断，是否该完善？
Matrix &Matrix::setRow(int row, double *values, int numValues) {
    for (int i = 0; i < cols_ && i < numValues; i++) {
        data_[row][i] = values[i];
    }
    return *this;
}

// 外部调用：获取矩阵列
// 使用完需要delete!!!!!!!!!!!
double *Matrix::getCol(int col) const {
    double *specificCol = new double[rows_]();
    for (int i = 0; i < rows_; i++) {
        specificCol[i] = data_[i][col];
    }
    return specificCol;
}

// 外部调用：设置矩阵列（hamocat新增numValues，原因同理见main.cpp）
Matrix &Matrix::setCol(int col, double *values, int numValues) {
    for (int i = 0; i < rows_ && i < numValues; i++) {
        data_[i][col] = values[i];
    }
    return *this;
}

// 外部调用：获取矩阵行数
int Matrix::getRows() const { return rows_; }

// 外部调用：设置矩阵行数（hamocat新增）
void Matrix::setRows(int rows) { rows_ = rows; }

// 外部调用：获取矩阵列数
int Matrix::getCols() const { return cols_; }

// 外部调用：设置矩阵列数（hamocat新增）
void Matrix::setCols(int cols) { cols_ = cols; }

// 外部调用：获取矩阵数据
double **Matrix::getData() const { return data_; }

// 外部调用：设置矩阵数据（hamocat新增）
void Matrix::setData(double **data) { data_ = data; }

// 打印矩阵（hamocat修改返回值为void，鉴于打印矩阵不需要返回原矩阵）
void Matrix::print() const {
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            std::cout << data_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// 矩阵加法
// 还不会做错误处理真是抱歉
Matrix Matrix::add(const Matrix &other) const {
    if (other.cols_ != cols_ || other.rows_ != rows_) {
        std::cout
            << "Error: can't add two matrices with different rows and cols"
            << std::endl;
        Matrix newMatrix;
        return newMatrix;
    }

    Matrix newMatrix(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            newMatrix.set(i, j, data_[i][j] + other.data_[i][j]);
        }
    }
    return newMatrix;
}

// 矩阵减法
Matrix Matrix::minus(const Matrix &other) const {
    if (other.cols_ != cols_ || other.rows_ != rows_) {
        std::cout
            << "Error: can't add two matrices with different rows and cols"
            << std::endl;
        Matrix newMatrix;
        return newMatrix;
    }

    Matrix newMatrix(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            newMatrix.set(i, j, data_[i][j] - other.data_[i][j]);
        }
    }
    return newMatrix;
}

// 矩阵点积(不要叫矩阵乘法了，需要有矩阵数乘重载*运算符)
Matrix Matrix::dotProduct(const Matrix &other) const {
    if (cols_ != other.rows_) {
        std::cout
            << "Error: can't multiply two matrices with unmatched rows and cols"
            << std::endl;
        Matrix newMatrix;
        return newMatrix;
    }

    Matrix newMatrix(rows_, other.cols_);
    for (int i = 0; i < rows_; i++) {
        Matrix tempMat(other);
        for (int j = 0; j < other.rows_; j++) {
            for (int p = 0; p < other.cols_; p++) {
                tempMat.data_[j][p] = other.data_[j][p] * data_[i][j];
            }
        }
        for (int q = 0; q < other.cols_; q++) {
            for (int t = 0; t < other.rows_; t++) {
                newMatrix.data_[i][q] += tempMat.data_[t][q];
            }
        }
    }
    return newMatrix;
}

// 矩阵转置
Matrix Matrix::transpose() const {
    Matrix transMat(cols_, rows_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_ ; j ++){
            transMat.data_[j][i] = data_[i][j];
        }
    }
    return transMat;
}

// 矩阵是否为方阵
bool Matrix::isSquare() const { return (rows_ == cols_) ? true : false; }

// bomb!!!(hamocat)
Matrix Matrix::bomb(int row, int col) const {
    Matrix tempMat(rows_ - 1, cols_ - 1);
    for (int it = 0, im = 0; it < rows_ - 1; it++, im++) {
        if (it == row) {
            im++;
        }
        const double *tempArr = data_[im];
        for (int jt = 0, jm = 0; jt < cols_ - 1; jt++, jm++) {
            if (jt == col) {
                jm++;
            }
            tempMat.set(it, jt, tempArr[jm]);
        }
    }
    return tempMat;
}

// 矩阵行列式
// 这样的错误处理会和非满秩矩阵冲突
double Matrix::determinant() const {
    if (!isSquare() || cols_ == 0) {
        std::cout << "Error: not a square matrix or is a zero matrix"
                  << std::endl;
        return 0;
    }
    double det = 0;
    if (cols_ > 2) {
        for (int i = 0; i < cols_; i++) {
            Matrix tempMat = bomb(0, i);
            det += pow(-1, i) * data_[0][i] * tempMat.determinant();
        }
    }
    if (cols_ == 2) {
        double a = get(0, 0), b = get(0, 1), c = get(1, 0), d = get(1, 1);
        det = a * d - b * c;
    }
    if (cols_ == 1) {
        det = get(0, 0);
    }

    return det;
}

// 矩阵求逆
Matrix Matrix::inverse() const {
    if (!determinant()) {
        std::cout << "Error: not a full rank matrix" << std::endl;
        Matrix invMat;
        return invMat;
    }
    if (rows_ == 1 && cols_ == 1){
        Matrix invMat(1, 1);
        invMat.data_[0][0] = 1 / data_[0][0];
        return invMat;
    }
    Matrix trans_adjMat(rows_, cols_);
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            Matrix tempMat = bomb(i, j);
            trans_adjMat.set(i, j, pow(-1, (i + j)) * tempMat.determinant());
        }
    }
    Matrix invMat = trans_adjMat.transpose() / determinant();
    return invMat;
}

// 重载运算符
Matrix operator+(const Matrix &thisMat, const Matrix &other) {
    Matrix tempMat = thisMat.add(other);
    return tempMat;
}
Matrix operator-(const Matrix &thisMat, const Matrix &other) {
    Matrix tempMat = thisMat.minus(other);
    return tempMat;
}
Matrix operator-(const Matrix &thisMat) {
    Matrix zeroMat(thisMat.rows_, thisMat.cols_);
    Matrix tempMat = zeroMat.minus(thisMat);
    return tempMat;
}
Matrix operator*(const Matrix &thisMat, const Matrix &other) {
    Matrix tempMat = thisMat.dotProduct(other);
    return tempMat;
}
Matrix operator*(const Matrix &thisMat, double num) {
    Matrix tempMat(thisMat.rows_, thisMat.cols_);
    for (int i = 0; i < thisMat.rows_; i++) {
        for (int j = 0; j < thisMat.cols_; j++) {
            tempMat.data_[i][j] = thisMat.data_[i][j] * num;
        }
    }
    return tempMat;
}
Matrix operator*(double num, const Matrix &thisMat) {
    Matrix tempMat(thisMat.rows_, thisMat.cols_);
    for (int i = 0; i < thisMat.rows_; i++) {
        for (int j = 0; j < thisMat.cols_; j++) {
            tempMat.data_[i][j] = thisMat.data_[i][j] * num;
        }
    }
    return tempMat;
}
Matrix operator/(const Matrix &thisMat, double num) {
    Matrix tempMat(thisMat.rows_, thisMat.cols_);
    for (int i = 0; i < thisMat.rows_; i++) {
        for (int j = 0; j < thisMat.cols_; j++) {
            tempMat.data_[i][j] = thisMat.data_[i][j] / num;
        }
    }
    return tempMat;
}
// 将赋值的内部实现由 判断值相等 改成 判断地址相等。原先为if(other != *this)
// 理由：即使左右值相等，也不影响operator=赋值操作照常进行！
// 但如果地址相等（是同一个对象），考虑到会将左值内存回收，而右值和左值在这个情况下是相同对象，
// 这样再进行赋值操作就有访问已被回收的内存空间的风险。
// 只是我通过if的判断恰好避免了这个情况发生，虽然用NaN这样离谱的数据会有问题，而用地址比较的理念就不会有这样的风险。
Matrix &Matrix::operator=(const Matrix &other) {
    if (&other != this) {
        double **newData_ = new double *[other.rows_];
        for (int i = 0; i < other.rows_; i++) {
            *(newData_ + i) = new double [other.cols_];
        }
        for (int i = 0; i < other.rows_; i++) {
            for (int j = 0; j < other.cols_; j++) {
                newData_[i][j] = other.data_[i][j];
            }
        }

        for (int i = 0; i < rows_; i++) {
            delete[] *(data_ + i);
        }
        delete[] data_;

        data_ = newData_;
        rows_ = other.rows_;
        cols_ = other.cols_;
        newData_ = nullptr;
    }
    return *this;
}
// 修复了内存泄漏：
// 如果只是为了指向同一个内存块，而不需要对其赋值，就没有必要为它动态分配内存空间。
Matrix &Matrix::operator=(Matrix &&other){
    if(&other != this){
        double **newData_ = other.data_;

        for (int i = 0; i < rows_; i++) {
            delete[] *(data_ + i);
        }
        delete[] data_;

        data_ = newData_;
        rows_ = other.rows_;
        cols_ = other.cols_;

        other.data_ = nullptr;
        other.rows_ = 0;
        other.cols_ = 0;
        newData_ = nullptr;
    }
    return *this;
}
bool Matrix::operator==(const Matrix &other) const{
    if (rows_ != other.rows_ || cols_ != other.cols_){
        return false;
    }else{
        for (int i = 0; i < rows_; i ++){
            for (int j = 0; j < cols_; j ++){
                if (data_[i][j] != other.data_[i][j]){return false;}
            }
        }
        return true;
    }
}
bool Matrix::operator!=(const Matrix &other) const{
    if (rows_ != other.rows_ || cols_ != other.cols_){
        return true;
    }else{
        for (int i = 0; i < rows_; i ++){
            for (int j = 0; j < cols_; j ++){
                if (data_[i][j] != other.data_[i][j]){return true;}
            }
        }
        return false;
    }
}
double Matrix::operator()(int row, int col) const{
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_){
        std::cout << "Error: index (" << row << ", " << col << ") is out of range" << std::endl;
        return 0;
    }
    return data_[row][col];
}
// 这段抄ai的，dummy是什么？
double &Matrix::operator()(int row, int col){
    static double dummy = 0;
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_){
        std::cout << "Error: index (" << row << ", " << col << ") is out of range" << std::endl;
        dummy = 0;
        return dummy;
    }
    return data_[row][col];
}
std::ostream &operator<<(std::ostream &os, const Matrix &matrix){
    for (int i = 0; i < matrix.rows_; i ++){
        os << "|";
        for (int j = 0; j < matrix.cols_; j ++){
            if (matrix.data_[i][j] > 0){os << " ";}
            os << std:: setw(3) << matrix.data_[i][j] << " ";
        }
        os << "|" << std::endl;
    }
    return os;
}
