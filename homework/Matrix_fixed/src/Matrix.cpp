// C++ 作业，矩阵类的实现
// author: hamocat (github: GaryGa)
// 关注永雏塔菲谢谢喵
// 鉴于给定的函数接口，矩阵结构通过最简单的二维数组实现，析构也用了手动delete，
// 很遗憾没有用到vector和智能指针等c++特性。
// 希望能抽出时间重写第二版，但这样的话原来很多接口就要修改了。
// 2026.11.22
//
// 下标约定（hamocat补充）：本实现中行、列下标统一从 0 开始，
// 即 get(0, 0) / m(0, 0) 是左上角元素，get(rows_ - 1, cols_ - 1) 是右下角元素。
// 作业模板 main.cpp 里的注释是按从 1 开始写的，这里以实际代码为准。
//
// 注：该文件Matrix_fixed由ai优化生成，可不着重看。
// 2026.11.26
#include "Matrix.hpp"
#include <iostream>
#include <cmath>

// 默认构造函数
Matrix::Matrix() : rows_(0), cols_(0), data_(nullptr), cursor_(0) {}

// 构造函数
// new需要做未正常分配内存的错误处理吗？
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(nullptr), cursor_(0) {
    // 行数或列数传成负数说明调用者写错了，直接按 0 处理，
    // 否则 new 会去申请一个天文数字大小的内存
    if (rows_ < 0){ rows_ = 0; }
    if (cols_ < 0){ cols_ = 0; }
    data_ = new double *[rows_];
    for (int i = 0; i < rows_; i ++){
        data_[i] = new double [cols_]();
    }
}

// 拷贝构造函数
Matrix::Matrix(const Matrix &other)
    : rows_(other.rows_), cols_(other.cols_), cursor_(other.cursor_) {
    data_ = new double *[rows_];
    for (int i = 0; i < rows_; i ++){
        // 原来漏了给每一行申请内存，直接往 data_[i] 里写就是野指针
        data_[i] = new double [cols_]();
        for (int j = 0; j < cols_; j ++){
            data_[i][j] = other.data_[i][j];
        }
    }
}

// 移动构造函数
Matrix::Matrix(Matrix &&other) noexcept
    : rows_(other.rows_), cols_(other.cols_), data_(other.data_), cursor_(other.cursor_) {
    // 这里只是把指针“偷”过来，千万不能 delete，否则 other 析构时会二次释放
    other.setRows(0);
    other.setCols(0);
    other.setData(nullptr);
    other.cursor_ = 0;
}

// 析构函数
Matrix::~Matrix(){
    if (data_){
        for (int i = 0; i < rows_; i ++){ delete [] data_[i]; }
        // 原来漏了这一句，行指针数组本身没有释放，每次析构都在泄漏
        delete [] data_;
        data_ = nullptr;
    }
}

// (链式)设置矩阵元素
Matrix &Matrix::set(int row, int col, double value){
    data_[row][col] = value;
    return *this;
}

// 获取矩阵元素
double Matrix::get(int row, int col) const{
    double value = data_[row][col];
    return value;
}

// 获取矩阵行
double *Matrix::getRow(int row) const{
    double* specificRow = data_[row];
    return specificRow;
}

// 设置矩阵行（hamocat新增numValues，原因见main.cpp）
Matrix &Matrix::setRow(int row, double *values, int numValues){
    for (int i = 0; i < cols_ && i < numValues; i ++){
        data_[row][i] = values[i];
    }

    return *this;
}

// 获取矩阵列
// 返回的是新申请的数组，调用者用完要自己 delete[]，否则会泄漏
double *Matrix::getCol(int col) const{
    // 原来这里只声明了一个未初始化的指针就往里写，是货真价实的越界写
    double* specificCol = new double [rows_]();
    for (int i = 0; i < rows_; i ++){
        specificCol[i] = data_[i][col];
    }
    return specificCol;
}

// 设置矩阵列（hamocat新增numValues，原因同理见main.cpp）
Matrix &Matrix::setCol(int col, double *values, int numValues){
    for (int i = 0; i < rows_ && i < numValues; i ++){
        data_[i][col] = values[i];
    }
    return *this;
}

// 获取矩阵行数
int Matrix::getRows() const{
    return rows_;
}

// 设置矩阵行数（hamocat新增）
void Matrix::setRows(int rows){
    rows_ = rows;
}

// 获取矩阵列数
int Matrix::getCols() const{
    return cols_;
}

// 设置矩阵列数（hamocat新增）
void Matrix::setCols(int cols){
    cols_ = cols;
}

// 获取矩阵数据
double **Matrix::getData() const{
    return data_;
}

// 设置矩阵数据（hamocat新增）
void Matrix::setData(double **data){
    data_ = data;
}

// 打印矩阵（hamocat修改返回值为void，鉴于打印矩阵不需要返回原矩阵）
void Matrix::print() const{
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            std::cout << data_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// 矩阵加法
// 还不会做错误处理真是抱歉
Matrix Matrix::add(const Matrix &other) const{
    if (other.cols_ != cols_ || other.rows_ != rows_){
        std::cout << "Error: can't add two matrices with different rows and cols" << std::endl;
        Matrix newMatrix;
        return newMatrix;
    }

    Matrix newMatrix(rows_, cols_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            newMatrix.set(i, j, data_[i][j] + other.data_[i][j]);
        }
    }
    return newMatrix;
}

// 矩阵乘法
// 原来的写法有四个问题：结果写成了转置、临时矩阵的下标用反、getCol 越界、
// 每一轮 for 申请的临时数组都没有释放。这里改回最直白的“行乘列”三重循环，
// i、j 是结果的行列，k 是相乘时要累加的公共维度
Matrix Matrix::multiply(const Matrix &other) const{
    if (cols_ != other.rows_){
        std::cout << "Error: can't multiply two matrices with unmatched rows and cols" << std::endl;
        Matrix newMatrix;
        return newMatrix;
    }

    Matrix newMatrix(rows_, other.cols_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < other.cols_; j ++){
            double sum = 0;
            for (int k = 0; k < cols_; k ++){
                sum += data_[i][k] * other.data_[k][j];
            }
            newMatrix.data_[i][j] = sum;
        }
    }

    return newMatrix;
}

// 矩阵转置
Matrix Matrix::transpose() const{
    Matrix transMat(cols_, rows_);
    // 原来的写法要先把一整列拷进临时数组，而 getCol 又是坏的；
    // 转置本来就是一次下标对调，直接双重循环就行，也不需要额外申请内存
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            transMat.data_[j][i] = data_[i][j];
        }
    }

    return transMat;
}

// 矩阵是否为方阵
bool Matrix::isSquare() const{
    return (rows_ == cols_) ? true: false;
}

// bomb!!!(hamocat)
// 划掉第 row 行、第 col 列之后剩下的余子式
Matrix Matrix::bomb(int row, int col)const{
    if (rows_ <= 1 || cols_ <= 1 || row < 0 || row >= rows_ || col < 0 || col >= cols_){
        std::cout << "Error: can't bomb a 1x1 matrix or the index is out of range" << std::endl;
        return Matrix();
    }

    Matrix tempMat(rows_ - 1, cols_ - 1);
    // 原来用 it / im 两个变量互相追赶来跳过被划掉的行列，容易写错也不好读；
    // 划掉第 row 行之后，原矩阵的行号 im 就是“it 小于 row 时取 it，否则取 it + 1”
    for (int it = 0; it < rows_ - 1; it ++){
        int im = (it < row) ? it : it + 1;
        for (int jt = 0; jt < cols_ - 1; jt ++){
            int jm = (jt < col) ? jt : jt + 1;
            tempMat.set(it, jt, data_[im][jm]);
        }
    }
    return tempMat;
}

// 矩阵行列式
// 这样的错误处理会和非满秩矩阵冲突
double Matrix::determinant() const{
    if (!isSquare() || rows_ == 0){
        std::cout << "Error: not a square matrix or is a zero matrix" << std::endl;
        return 0;
    }

    // 1 阶、2 阶是递归的出口，直接按定义算
    if (rows_ == 1){
        return data_[0][0];
    }
    if (rows_ == 2){
        return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
    }

    // 按第一行展开：det = Σ (-1)^j * a(0, j) * M(0, j)
    // 原来的代码里写的是 data_[1][i] 和 get(1, 1) 这种从 1 开始的下标，
    // 放到 0 开始的矩阵上直接越界，算出来的东西自然也是错的
    double det = 0;
    for (int j = 0; j < cols_; j ++){
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        Matrix tempMat = bomb(0, j);
        det += sign * data_[0][j] * tempMat.determinant();
    }

    return det;
}

// 矩阵求逆
// 逆矩阵 = 伴随矩阵 / 行列式，伴随矩阵就是代数余子式矩阵的转置
Matrix Matrix::inverse() const{
    if (!isSquare() || rows_ == 0){
        std::cout << "Error: not a square matrix" << std::endl;
        return Matrix();
    }

    double det = determinant();
    // 原来的判断写反了，if (determinant()) 变成“只要可逆就报错”，
    // 而且既没有转置也没有除以行列式，算出来的是代数余子式矩阵本身
    // 浮点数和 0 直接比较不保险，这里留一个很小的阈值
    if (std::fabs(det) < 1e-12){
        std::cout << "Error: not a full rank matrix" << std::endl;
        return Matrix();
    }

    Matrix invMat(rows_, cols_);
    // 1 阶矩阵没有余子式可划，伴随矩阵就是 [[1]]，单独处理
    if (rows_ == 1){
        invMat.data_[0][0] = 1.0 / det;
        return invMat;
    }

    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            // 伴随矩阵第 (i, j) 项是代数余子式 (j, i)，所以 bomb 的下标要反过来传
            Matrix tempMat = bomb(j, i);
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            invMat.data_[i][j] = sign * tempMat.determinant() / det;
        }
    }

    return invMat;
}

// 重载运算符 +，实现矩阵加法
Matrix Matrix::operator+(const Matrix &other) const{
    // 形状检查的错误信息只留在 add() 里，这里不必再抄一遍
    return add(other);
}

// 重载运算符 -，实现矩阵减法
Matrix Matrix::operator-(const Matrix &other) const{
    if (rows_ != other.rows_ || cols_ != other.cols_){
        std::cout << "Error: can't subtract two matrices with different rows and cols" << std::endl;
        return Matrix();
    }

    Matrix newMatrix(rows_, cols_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            newMatrix.data_[i][j] = data_[i][j] - other.data_[i][j];
        }
    }

    return newMatrix;
}

// 重载一元运算符 -，实现矩阵取负
Matrix Matrix::operator-() const{
    Matrix newMatrix(rows_, cols_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            newMatrix.data_[i][j] = -data_[i][j];
        }
    }

    return newMatrix;
}

// 重载运算符 *，实现矩阵乘法
Matrix Matrix::operator*(const Matrix &other) const{
    return multiply(other);
}

// 重载运算符 /，实现矩阵逐元素相除
// 矩阵其实没有天然的除法，这里按 hamocat 的选择做成逐元素相除（Hadamard 除法）：
// C(i, j) = A(i, j) / B(i, j)，所以两侧必须是同型矩阵
Matrix Matrix::operator/(const Matrix &other) const{
    if (rows_ != other.rows_ || cols_ != other.cols_){
        std::cout << "Error: can't divide two matrices with different rows and cols" << std::endl;
        return Matrix();
    }

    Matrix newMatrix(rows_, cols_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            if (other.data_[i][j] == 0){
                std::cout << "Error: divide by zero at (" << i << ", " << j << ")" << std::endl;
                return Matrix();
            }
            newMatrix.data_[i][j] = data_[i][j] / other.data_[i][j];
        }
    }

    return newMatrix;
}

// 重载运算符 =，实现拷贝赋值
Matrix &Matrix::operator=(const Matrix &other){
    // 自己给自己赋值（m = m）时，如果先把旧内存放掉，数据就跟着没了
    if (this == &other){
        return *this;
    }

    // 先按 other 的尺寸申请新内存、拷完数据，再释放旧内存：
    // 这样即使 new 中途抛出异常，*this 也还是原来那个完好的矩阵
    double **newData = new double *[other.rows_];
    for (int i = 0; i < other.rows_; i ++){
        newData[i] = new double [other.cols_]();
        for (int j = 0; j < other.cols_; j ++){
            newData[i][j] = other.data_[i][j];
        }
    }

    if (data_){
        for (int i = 0; i < rows_; i ++){ delete [] data_[i]; }
        delete [] data_;
    }

    rows_ = other.rows_;
    cols_ = other.cols_;
    data_ = newData;
    cursor_ = other.cursor_;

    return *this;
}

// 重载运算符 =，实现移动赋值
// 和拷贝赋值的区别：不用逐个元素拷贝，直接把 other 的指针接管过来，
// other 随后变成一个空矩阵，移动语义省掉的就是这一次深拷贝
Matrix &Matrix::operator=(Matrix &&other){
    if (this == &other){
        return *this;
    }

    if (data_){
        for (int i = 0; i < rows_; i ++){ delete [] data_[i]; }
        delete [] data_;
    }

    rows_ = other.getRows();
    cols_ = other.getCols();
    data_ = other.getData();
    cursor_ = other.cursor_;

    other.setRows(0);
    other.setCols(0);
    other.setData(nullptr);
    other.cursor_ = 0;

    return *this;
}

// 重载运算符 ==，实现矩阵比较
// 这里直接用 == 比较 double，作业给的数都是能精确表示的小整数；
// 真实工程里应该写成 fabs(a - b) < eps
bool Matrix::operator==(const Matrix &other) const{
    if (rows_ != other.rows_ || cols_ != other.cols_){
        return false;
    }

    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            if (data_[i][j] != other.data_[i][j]){
                return false;
            }
        }
    }

    return true;
}

// 重载运算符 !=，实现矩阵比较
bool Matrix::operator!=(const Matrix &other) const{
    return !(*this == other);
}

// 重载运算符 ()，实现矩阵元素的只读访问
double Matrix::operator()(int row, int col) const{
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_){
        std::cout << "Error: index (" << row << ", " << col << ") is out of range" << std::endl;
        return 0;
    }

    return data_[row][col];
}

// 重载运算符 ()，实现矩阵元素的读写访问
// 只有返回引用才能写 m(0, 0) = 1.5
double &Matrix::operator()(int row, int col){
    // 越界时不能返回野指针，这里返回一个静态哑元，保证程序不会直接崩掉；
    // 但写进哑元的值下一次越界就会被覆盖，所以它只适合“读一下不至于崩”的场合
    static double dummy = 0;
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_){
        std::cout << "Error: index (" << row << ", " << col << ") is out of range" << std::endl;
        dummy = 0;
        return dummy;
    }

    return data_[row][col];
}

// 重载运算符 <<，实现矩阵的输入：按行优先顺序，每次写入一个元素
Matrix &Matrix::operator<<(double value){
    if (rows_ <= 0 || cols_ <= 0){
        std::cout << "Error: can't write into an empty matrix" << std::endl;
        return *this;
    }

    // 写满一轮就从头覆盖，免得第二次输入时每个 << 都只报错
    if (cursor_ >= rows_ * cols_){
        std::cout << "Matrix is full, restart from (0, 0)" << std::endl;
        cursor_ = 0;
    }

    // 一维的 cursor_ 映射回二维下标：第 cursor_ / cols_ 行、第 cursor_ % cols_ 列
    data_[cursor_ / cols_][cursor_ % cols_] = value;
    cursor_ ++;

    return *this;
}

// 重载流运算符 <<，实现矩阵的输出
std::ostream &operator<<(std::ostream &os, const Matrix &matrix){
    for (int i = 0; i < matrix.rows_; i ++){
        for (int j = 0; j < matrix.cols_; j ++){
            os << matrix.data_[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}
