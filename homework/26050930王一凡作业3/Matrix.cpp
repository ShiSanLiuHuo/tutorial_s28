#include "Matrix.hpp"
#include <cmath>
#include <iomanip>
#include <utility>

void Matrix::allocate() {
    data = new double*[rows_];
    for (int i = 0; i < rows_; ++i)
        data[i] = new double[cols_]();
}

void Matrix::freeData() {
    for (int i = 0; i < rows_; ++i)
        delete[] data[i];
    delete[] data;
}

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
    if (rows <= 0 || cols <= 0)
        throw std::invalid_argument("Matrix size must be positive");
    allocate();
}

Matrix::Matrix(const Matrix &other) : rows_(other.rows_), cols_(other.cols_) {
    allocate();
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            data[i][j] = other.data[i][j];
}

Matrix& Matrix::operator=(const Matrix &other) {
    if (this == &other) return *this;
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        freeData();
        rows_ = other.rows_;
        cols_ = other.cols_;
        allocate();
    }
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            data[i][j] = other.data[i][j];
    return *this;
}

Matrix::~Matrix() { freeData(); }

Matrix& Matrix::set(int row, int col, double val) {
    if (row < 1 || row > rows_ || col < 1 || col > cols_)
        throw std::out_of_range("set index out of range");
    data[row - 1][col - 1] = val;
    return *this;
}

void Matrix::print() const {
    std::cout << " = \n";
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j)
            std::cout << std::setw(10) << data[i][j];
        std::cout << std::endl;
    }
}

double& Matrix::operator()(int row, int col) {
    if (row < 1 || row > rows_ || col < 1 || col > cols_)
        throw std::out_of_range("index out of range");
    return data[row - 1][col - 1];
}

const double& Matrix::operator()(int row, int col) const {
    if (row < 1 || row > rows_ || col < 1 || col > cols_)
        throw std::out_of_range("index out of range");
    return data[row - 1][col - 1];
}

Matrix Matrix::operator+(const Matrix &o) const {
    if (rows_ != o.rows_ || cols_ != o.cols_)
        throw std::invalid_argument("size mismatch for +");
    Matrix r(rows_, cols_);
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            r.data[i][j] = data[i][j] + o.data[i][j];
    return r;
}

Matrix Matrix::operator-(const Matrix &o) const {
    if (rows_ != o.rows_ || cols_ != o.cols_)
        throw std::invalid_argument("size mismatch for -");
    Matrix r(rows_, cols_);
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            r.data[i][j] = data[i][j] - o.data[i][j];
    return r;
}

Matrix Matrix::operator*(const Matrix &o) const {
    if (cols_ != o.rows_)
        throw std::invalid_argument("size mismatch for *");
    Matrix r(rows_, o.cols_);
    for (int i = 0; i < r.rows_; ++i)
        for (int j = 0; j < r.cols_; ++j) {
            double s = 0;
            for (int k = 0; k < cols_; ++k)
                s += data[i][k] * o.data[k][j];
            r.data[i][j] = s;
        }
    return r;
}

Matrix Matrix::operator/(const Matrix &o) const {
    if (rows_ != o.rows_ || cols_ != o.cols_)
        throw std::invalid_argument("size mismatch for /");
    Matrix r(rows_, cols_);
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j) {
            if (o.data[i][j] == 0)
                throw std::domain_error("division by zero");
            r.data[i][j] = data[i][j] / o.data[i][j];
        }
    return r;
}

Matrix Matrix::operator-() const {
    Matrix r(rows_, cols_);
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            r.data[i][j] = -data[i][j];
    return r;
}

Matrix Matrix::transpose() const {
    Matrix r(cols_, rows_);
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            r.data[j][i] = data[i][j];
    return r;
}

double Matrix::determinant() const {
    if (rows_ != cols_)
        throw std::invalid_argument("determinant needs square matrix");
    int n = rows_;
    Matrix a(*this);
    double det = 1.0;
    for (int i = 0; i < n; ++i) {
        int piv = i;
        for (int k = i + 1; k < n; ++k)
            if (std::fabs(a.data[k][i]) > std::fabs(a.data[piv][i])) piv = k;
        if (std::fabs(a.data[piv][i]) < 1e-12) return 0.0;
        if (piv != i) { std::swap(a.data[i], a.data[piv]); det = -det; }
        det *= a.data[i][i];
        for (int k = i + 1; k < n; ++k) {
            double f = a.data[k][i] / a.data[i][i];
            for (int j = i; j < n; ++j)
                a.data[k][j] -= f * a.data[i][j];
        }
    }
    return det;
}

Matrix Matrix::inverse() const {
    if (rows_ != cols_)
        throw std::invalid_argument("inverse needs square matrix");
    int n = rows_;
    Matrix a(*this);
    Matrix inv(n, n);
    for (int i = 0; i < n; ++i) inv.data[i][i] = 1.0;
    for (int i = 0; i < n; ++i) {
        int piv = i;
        for (int k = i + 1; k < n; ++k)
            if (std::fabs(a.data[k][i]) > std::fabs(a.data[piv][i])) piv = k;
        if (std::fabs(a.data[piv][i]) < 1e-12)
            throw std::domain_error("matrix is singular");
        if (piv != i) {
            std::swap(a.data[i], a.data[piv]);
            std::swap(inv.data[i], inv.data[piv]);
        }
        double d = a.data[i][i];
        for (int j = 0; j < n; ++j) {
            a.data[i][j]   /= d;
            inv.data[i][j] /= d;
        }
        for (int k = 0; k < n; ++k) {
            if (k == i) continue;
            double f = a.data[k][i];
            for (int j = 0; j < n; ++j) {
                a.data[k][j]   -= f * a.data[i][j];
                inv.data[k][j] -= f * inv.data[i][j];
            }
        }
    }
    return inv;
}

bool Matrix::operator==(const Matrix &o) const {
    if (rows_ != o.rows_ || cols_ != o.cols_) return false;
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            if (std::fabs(data[i][j] - o.data[i][j]) > 1e-9) return false;
    return true;
}

bool Matrix::operator!=(const Matrix &o) const { return !(*this == o); }

void Matrix::assignFrom(std::initializer_list<double> values) {
    if ((int)values.size() < rows_ * cols_)
        throw std::invalid_argument("not enough values");
    auto it = values.begin();
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            data[i][j] = *it++;
}

std::ostream& operator<<(std::ostream &os, const Matrix &m) {
    for (int i = 1; i <= m.rows(); ++i) {
        for (int j = 1; j <= m.cols(); ++j)
            os << std::setw(10) << m(i, j);
        os << '\n';
    }
    return os;
}
