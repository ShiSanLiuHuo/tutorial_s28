#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <iostream>
#include <initializer_list>
#include <stdexcept>

class Matrix {
public:
    Matrix(int rows, int cols);
    Matrix(const Matrix &other);
    Matrix& operator=(const Matrix &other);
    ~Matrix();

    Matrix& set(int row, int col, double val);
    void print() const;

    double& operator()(int row, int col);
    const double& operator()(int row, int col) const;

    Matrix operator+(const Matrix &o) const;
    Matrix operator-(const Matrix &o) const;
    Matrix operator*(const Matrix &o) const;
    Matrix operator/(const Matrix &o) const;
    Matrix operator-() const;

    Matrix transpose() const;
    Matrix inverse() const;

    bool operator==(const Matrix &o) const;
    bool operator!=(const Matrix &o) const;

    double determinant() const;
    void assignFrom(std::initializer_list<double> values);

    int rows() const { return rows_; }
    int cols() const { return cols_; }

private:
    int rows_, cols_;
    double **data;

    void allocate();
    void freeData();
};

std::ostream& operator<<(std::ostream &os, const Matrix &m);

#endif
