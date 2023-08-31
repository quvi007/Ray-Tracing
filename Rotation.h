#include "Matrix.h"
#include "Point.h"

#ifndef ROTATION_H
#define ROTATION_H

class Rotation {
private:
    double angle;
    double ax, ay, az;
    Matrix matrix;
    friend ostream &operator<<(ostream &, const Rotation &);
    friend istream &operator>>(istream &, Rotation &);
public:
    Rotation();
    Rotation(double, double, double, double);
    Rotation(const Rotation &);
    
    Rotation &operator=(const Rotation &);    

    double getAngle() const;
    double getAx() const;
    double getAy() const;
    double getAz() const;
    Matrix getMatrix() const;

    void setAngle(double);
    void setAx(double);
    void setAy(double);
    void setAz(double);

    static Point R(const Point &, const Point &, double);
};

#endif