#include <iostream>
#include <iomanip>

using namespace std;

#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
private:
    int n, m;
    double **mat;
    friend Matrix operator*(double, const Matrix &);
    friend ostream &operator<<(ostream &, const Matrix &);
    friend istream &operator>>(istream &, Matrix &);
public:
    Matrix();
    Matrix(int, int);
    Matrix(int, int, const double *);
    Matrix(const Matrix &);
    ~Matrix();
    Matrix &operator=(const Matrix &);
    Matrix &operator+=(const Matrix &);
    Matrix &operator-=(const Matrix &);
    Matrix &operator*=(const Matrix &);
    Matrix &operator*=(double);
    double *operator[](int);
    
    Matrix operator+(const Matrix &) const;
    Matrix operator-(const Matrix &) const;
    Matrix operator*(const Matrix &) const;
    Matrix operator*(double) const;
    int getN() const;
    int getM() const;

    static Matrix Identity(int);
};

#endif