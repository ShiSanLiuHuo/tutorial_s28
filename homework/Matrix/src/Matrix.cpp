#include"Matrix.hpp"
#include<cmath>

Matrix::Matrix(int rows,int cols):rows(rows),cols(cols){
    data=new double*[rows];
    for(int i=0;i<rows;i++){
        data[i] = new double[cols];
    }
}

Matrix::~Matrix(){
    for(int i=0;i<rows;i++){
        delete[] data[i];
    }
    delete[] data;
}

Matrix Matrix::operator+(const Matrix &o) const{
    Matrix res(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
        res.data[i][j]=data[i][j]+ o.data[i][j];}
    }
    return res;
}
Matrix Matrix::operator-(const Matrix &o) const{
    Matrix res(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
        res.data[i][j]=data[i][j]- o.data[i][j];
    }
    return res;
}

Matrix Matrix::operator*(const Matrix &o) const{
    Matrix res(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<o.cols;j++){
            for(int k=0;k<o.rows;k++){
                res.data[i][j]=data[i][k]*data[k][j];
            }
        }
    }
    return res;
}

Matrix Matrix::operator/(const Matrix &o) const{
    return *this*o.inverse();
}

Matrix Matrix::operator-() const{
    Matrix res(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
        res.data[i][j]=-data[i][j];}
    }
    return res;
}

Matrix Matrix::transpose() const{
    Matrix res(cols,rows);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
        res.data[i][j]=data[j][i];
    }
    return res;
}

Matrix Matrix::inverse() const{
    Matrix res(cols,rows);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)
        res.data[i][j]=data[j][i]/determinant();
    }
    return res;
}

double Matrix::determinant() const{
    return data[1][1]*data[0][0]-data[0][1]*data[1][0];
}

bool Matrix::operator==(const Matrix &o) const{
    if(rows!=o.rows||cols!=o.cols){
        return false;
    }
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(std::fabs(data[i][j]-o.data[i][j])>1e-6){
                return false;
            }
        }
    }
    return true;
}

bool  Matrix::operator!=(const Matrix &o) const{
    return !(*this==o);
}


