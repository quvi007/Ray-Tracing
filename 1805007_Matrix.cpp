#include "1805007_Matrix.h"

Matrix::Matrix() : Matrix(0, 0) {}

Matrix::Matrix(int n, int m) : n{n}, m{m} {
    mat = new double*[n];
    for (int i = 0; i < n; ++i) {
        mat[i] = new double[m];
    }
}

Matrix::Matrix(int n, int m, const double *mat) : Matrix(n, m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            this->mat[i][j] = *(mat + i * m + j);
        }
    }
}

Matrix::Matrix(const Matrix &matrix) : Matrix(matrix.n, matrix.m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            mat[i][j] = matrix.mat[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < n; ++i) {
        delete[] mat[i];
    }
    delete[] mat;
}

Matrix &Matrix::operator=(const Matrix &rhs) {
    for (int i = 0; i < n; ++i) {
        delete[] mat[i];
    }
    delete[] mat;
    n = rhs.n;
    m = rhs.m;
    mat = new double*[n];
    for (int i = 0; i < n; ++i) {
        mat[i] = new double[m];
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            mat[i][j] = rhs.mat[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator+=(const Matrix &rhs) {
    (*this) = (*this) + rhs;
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &rhs) {
    (*this) = (*this) - rhs;
    return *this;
}

Matrix &Matrix::operator*=(const Matrix &rhs) {
    (*this) = (*this) * rhs;
    return *this;
}
    
Matrix &Matrix::operator*=(double s) {
    (*this) = (*this) * s;
    return *this;
}

Matrix Matrix::operator+(const Matrix &obj) const {
    Matrix ans(n, m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ans.mat[i][j] = mat[i][j] + obj.mat[i][j];
        }
    }
    return ans;
}

Matrix Matrix::operator-(const Matrix &obj) const {
    Matrix ans(n, m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ans.mat[i][j] = mat[i][j] - obj.mat[i][j];
        }
    }
    return ans;
}

Matrix Matrix::operator*(const Matrix &obj) const {
    Matrix ans(n, obj.m);
    for (int i = 0; i < ans.n; ++i) {
        for (int j = 0; j < ans.m; ++j) {
            ans.mat[i][j] = 0;
            for (int k = 0; k < m; ++k) {
                ans.mat[i][j] += mat[i][k] * obj.mat[k][j];
            }
        }
    }
    return ans;
}

Matrix Matrix::operator*(double s) const {
    Matrix ans = (*this);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ans.mat[i][j] *= s;
        }
    }
    return ans;
}

double *Matrix::operator[](int k) {
    return mat[k];
}

int Matrix::getN() const {
    return n;
}

int Matrix::getM() const {
    return m;
}

Matrix Matrix::Identity(int n) {
    Matrix ans(n, n);
    for (int i = 0; i < n; ++i) {
        ans[i][i] = 1;
    }
    return ans;
}

Matrix operator*(double s, const Matrix &matrix) {
    return matrix * s;
}

ostream &operator<<(ostream &os, const Matrix &matrix) {
    for (int i = 0; i < matrix.n; ++i) {
        for (int j = 0; j < matrix.m; ++j) {
            cout << fixed << setprecision(7) << matrix.mat[i][j] << " ";
        }
        if (i < matrix.n - 1) cout << "\n";
    }
    return os;
}

istream &operator>>(istream &is, Matrix &matrix) {
    for (int i = 0; i < matrix.n; ++i) {
        for (int j = 0; j < matrix.m; ++j) {
            is >> matrix.mat[i][j];
        }
    }
    return is;
}

