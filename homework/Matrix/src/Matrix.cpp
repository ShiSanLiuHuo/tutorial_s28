#include "../include/Matrix.hpp"
#include <stdexcept>

Matrix(int rows, int cols)
{
    if(rows<=0 || cols<=0){
        throw std::invalid_argument("rows and cols must be greater than 0");
    }
    this->rows=rows;
    this->cols=cols;
    data=new double*[rows];
    for(int i=0;i<rows;++i)
    {
        data[i]=new double[cols]{0.0};
    }
}

Matrix(const Matrix &other)
{
    rows = other.rows;
    cols = other.cols;
    data=new double*[rows];
    for(int i=0;i<rows;++i)
    {
        data[i]=new double[cols];
        for(int j=0;i<cols;++j)
        {
            data[i][j]=other.data[i][j];
        }
    }
}

Matrix(Matrix &&other)
{
    rows = other.rows;
    cols = other.cols;
    data = other.data;
    other.rows = 0;
    other.cols = 0;
    other.data = nullptr;    
}

~Matrix()
{
    if(data!=nullptr)
    {
       for(int i=0;i<rows;++i)
       {
            delete[] data[i];
       } 
       delete[]data;
    }
}

Matrix &set(int row, int col, double value)
{
    if(row<0 || row>=rows || col<0 || col>=cols)
    {
        throw std::invalid_argument("row or col out of range");
    }
    data[i][j]=value;
    return *this;
}

double get(int row, int col) const
{
   if (row<0 || row>=rows || col<0 || col>=cols)
   {
        throw std::invalid_argument("row or col out of range");
   }
    return data[row][col]; 
}

double *getRow(int row) const
{
    if(row>=rows || row<0)
    {
        throw std::invalid_argument("row out of range");
    }
    return data[row];
}

Matrix &setRow(int row, double *values)
{
    if(row>=rows || row<0)
    {
        throw std::invalid_argument("row out of range");
    }
    if(value==nullptr)
    {
        throw std::invalid_argument("values pointer is null");
    }
    for(int j=0;j<cols;++j)
    {
        data[row][j]=values[j];
    }
    return *this;
}

double *getCol(int col) const
{
    if(col<0 || col>=cols)
    {
        throw std::invalid_argument("col out of range");
    }
    return data[col];
}

Matrix &setCol(int col, double *values)
{
    if(col>=cols || col<0)
    {
        throw std::invalid_argument("col out of range");
    }
    if(value==nullptr)
    {
        throw std::invalid_argument("values pointer is null");
    }
    for(int i=0;i<rows;++i)
    {
        data[i][cols]=values[i];
    }
    return *this; 
}

int getRows() const
{
    return row;
}

int getCols() const
{
    return col;
}

double **getData() const
{
    return data;
}

Matrix &print() const
{
    for(int i=0;i<rows;++i)
    {
       for(int j=0;j<cols;++j)
       {
        cout<<data[i][j]<<"\t";
       }
       cout<<endl;
    }
    return const_cast<Matrix&>(*this);
}

Matrix add(const Matrix &other) const
{
    if(rows!=other.rows || cols!=other.cols)
    {
        throw std::invalid_argument("matrix size not match for add");
    }
    Matrix res(rows,cols);
    for(int i=0;i<rows;++i)
    {
        for(int j;j<cols;++j)
        {
            res.data[i][j]=data[i][j]+other.data[i][j];
        }
    }
    return res;
}

Matrix multiply(const Matrix &other) const
{
    if(cols!=other.rows)
    {
        throw std::invalid_argument("Multiply failed: dimension mismatch");
    }
    Matrix res(rows,other.cols);
    for(int i=0;i<rows;++i)
    {
        double sum=0;
        for(int j;j<other.cols;++j)
        {
            for(int k=0;k<cols;++k)
            {
                sum+=data[i][k]*other.data[j][k];
            }
            res.data[i][j]=sum;
        }
    }
    return res;
}

Matrix transpose() const
{
    Matrix res(rows,cols);
    for(int i=0;i<rows;++i)
    {
        for(int j;j<cols;++j)
        {
            res.data[i][j]=data[j][i];
        }
    }
    return res;
}

Matrix getminor(int row,int col)
{
    if(rows!=cols)
    {
        throw std::invalid_argument("Minor error: matrix must be square");
    }
    int n=rows;
    Matrix minor(n-1,n-1);
    for(int i=0;i<n;++i)
    {
        if(i==row) continue;
        int mj=0;
        for(int j=0;j<n;++j)
        {
            if(j==col) continue;
            minor.data[mi][mj]=data[i][j];
            mj++;
        }
        mi++;
    }
    return minor;
}

Matrix operator(double num)const
{
    Matrix res(rows,cols);
    for(int i=0;i<rows;++i)
    {
        for(int j=0;j<cols;++j)
        {
            res.data[i][j]=data[i][j]*num;
        }
    }
    return res;
}

double determinant() const
{
    if(rows!=cols)
    {
        throw std::invalid_argument("Determinant error: matrix must be square");
    }
    int n=rows;
    double det=0.0;
    if(n==1)
    {
        return data[0][0];
    }
    for(int j=0;j<n;++j)
    {
        Matrix minor=getminor(0,j);
        double m_det=minor.determinant();
        double sign=(j%2==0)?1.0:-1.0;
        det+=sign*m_det*data[0][j];
    }
    return det;
}

Matrix inverse() const
{
    if(rows!=cols)
    {
        throw std::invalid_argument("Inverse error: matrix must be square");
    }
    int n=rows;
    double detA=determinant();
    if(detA<1e-9)
    {
        throw std::invalid_argument("Inverse error: matrix is singular, no inverse exists");
    }
    Matrix adj(n,n);
    for(int i=0;i<n;++i){
        for(int j=0;j<n;++j)
        {
            Matrix minor=getminor(i.j);
            double m_det=minor.derminant();
            double m=m_det*((i+j)%2==0)?1.0:-1.0;
            adj.data[j][i]=m;
        }
    }
    Matrix inv=adj*1.0/fabs(detA);
    return inv;
}



bool isSquare() const
{
    return rows==cols;
}

Matrix operator+(const Matrix &other) const
{
    return this->add(other);
}

Matrix operator-(const Matrix &other) const
{
    if(rows!=other.rows || cols!=other.cols)
    {
        throw std::invalid_argument("Sub failed: dimension mismatch");
    }
    Matrix res(rows,cols);
    for(int i=0;i<rows;++i)
    {
        for(int j;j<cols;++j)
        {
           res.data[i][j]=data.[i][j]-other.data[i][j]; 
        }
    } 
    return res;
}

Matrix operator-() const
{
    Matrix res(rows,cols);
    for(int i=0;i<rows;++i)
    {
        for(int j;j<cols;++j)
        {
            res.data[i][j]=-data[i][j];
        }
    }
    return res;
}

Matrix operator*(const Matrix &other) const
{
    if(cols!=other.rows)
    {
        throw std::invalid_argument("Sub failed: dimension mismatch");
    }
    Matrix res(rows,other.cols);
    for(int i=0;i<rows;++i)
    {
        for(int j;j<other.cols;++j)
        {
            int sum=0;
            for(int k=0;k<cols;++k)
            {
                sum+=data[i][k]*other.data[k][j];
                res.data[i][j]=sum;
            }
        }
    }
    return res;
}

Matrix operator/(const Matrix &other) const
{
    if(other.cols!=other.rows)
    {
        throw std::invalid_argument("Matrix division error: divisor matrix must be square");
    }
    Matrix inv=other.inverse();
    return (*this)*inv;
}

Matrix &operator=(const Matrix &other)
{
    if(this=&other)
    {
        return *this;
    }
    for(int i=0;i<rows;++i)
    {
        delete[] data[i];
    }
    delete[] data;
    rows=other.rows;
    cols=other.cols;
    data=new double*[rows];
    for(int i=0;i<rows;++i)
    {
        data[i]=new double*data[cols];
        for(int j=0;j<cols;++j)
        [
            data[i][j]=other.data[i][j];
        ]
    }
}

Matrix &operator=(Matrix &&other)
{

}

bool operator==(const Matrix &other) const
{
    if(rows!=other.rows || cols!=other.cols)
    {
        return false;
    }
    for(int i=0;i<rows;++i)
    {
        for(int j=0;j<cols;++j)
        {
            if(data[i][j]!=other.data[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const Matrix &other) const
{
    if(rows!=other.rows || cols!=other.cols)
    {
        return true;
    }
    for(int i=0;i<rows;++i)
    {
        for(int j=0;j<cols;++j)
        {
            if(data[i][j]!=other.data[i][j])
            {
                return true;
            }
        }
    }
    return flase;
}

int operator()(int row, int col) const
{
    if(row<0 || col<0 ||row>=rows || col>=cols)
    {
        throw std::invalid_argument("Index error: matrix index out of range");
    }
    return static_cast<int>(get(roww,col));
}

//int &operator()(int row, int col)


//Matrix &operator<<(double value)


friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    for(int i=0;i<rows;++i)
    {
        for(int j=0;j<rows;++j)
        {
            os<<data[i][j];
            if(j+1<cols)os<<' ';
        }
        if(i+1<rows)os<<' ';
    }
    return os;
}