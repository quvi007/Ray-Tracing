#include "Matrix.h"

#ifndef POINT_H
#define POINT_H

class Point {
private:
    double px, py, pz, w;
    Matrix matrix;
    friend Point operator*(double, const Point &);
    friend ostream &operator<<(ostream &, const Point &);
    friend istream &operator>>(istream &, Point &);
public:
    Point();
    Point(double, double, double);
    Point(double, double, double, double);
    Point(const Point &);
    Point(const Matrix &);
    
    Point &operator=(const Point &);
    Point operator+(const Point &) const;
    Point operator-(const Point &) const;
    Point operator*(const Point &) const;
    Point operator*(double) const;

    double getPx() const;
    double getPy() const;
    double getPz() const;
    double getW() const;
    Matrix getMatrix() const;

    void setPx(double);
    void setPy(double);
    void setPz(double);
    void setW(double);

    Point transform(const Matrix &) const;
    void normalize();

    Point scaleDown() const;

    static double dot(const Point &, const Point &);
};

#endif