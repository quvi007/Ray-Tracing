#include "Rotation.h"
#include <cmath>

#define RAD(x) (x * acos(-1.0) / 180.0)

Rotation::Rotation() : Rotation(0, 0, 0, 0) {}

Rotation::Rotation(double angle, double ax, double ay, double az) : angle{angle}, ax{ax}, ay{ay}, az{az} {
    Point a(ax, ay, az);
    a.normalize();
    Point c1, c2, c3;
    c1 = R(Point(1, 0, 0), a, angle);
    c2 = R(Point(0, 1, 0), a, angle);
    c3 = R(Point(0, 0, 1), a, angle);

    double m[][4] = {{c1.getPx(), c2.getPx(), c3.getPx(), 0}, {c1.getPy(), c2.getPy(), c3.getPy(), 0}, {c1.getPz(), c2.getPz(), c3.getPz(), 0}, {0, 0, 0, 1}};
    matrix = Matrix(4, 4, (const double *) m);
}

Rotation::Rotation(const Rotation &rotation) : Rotation(rotation.angle, rotation.ax, rotation.ay, rotation.az) {}

Rotation &Rotation::operator=(const Rotation &rhs) {
    angle = rhs.angle;
    ax = rhs.ax;
    ay = rhs.ay;
    az = rhs.az;
    matrix = rhs.matrix;
    return *this;
}

double Rotation::getAngle() const {
    return angle;
}

double Rotation::getAx() const {
    return ax;
}

double Rotation::getAy() const {
    return ay;
}

double Rotation::getAz() const {
    return az;
}

Matrix Rotation::getMatrix() const {
    return matrix;
}

void Rotation::setAngle(double angle) {
    (*this) = Rotation(angle, ax, ay, az);
}

void Rotation::setAx(double ax) {
    (*this) = Rotation(angle, ax, ay, az);
}

void Rotation::setAy(double ay) {
    (*this) = Rotation(angle, ax, ay, az);
}

void Rotation::setAz(double az) {
    (*this) = Rotation(angle, ax, ay, az);
}

Point Rotation::R(const Point &x, const Point &a, double angle) {
    return cos(RAD(angle)) * x + (1 - cos(RAD(angle))) * (Point::dot(a, x)) * a + sin(RAD(angle)) * (a * x);
}

ostream &operator<<(ostream &os, const Rotation &rotation) {
    os << fixed << setprecision(7) << rotation.angle << " " << rotation.ax << " " << rotation.ay << " " << rotation.az;
    return os;
}

istream &operator>>(istream &is, Rotation &rotation) {
    is >> rotation.angle >> rotation.ax >> rotation.ay >> rotation.az;
    rotation = Rotation(rotation.angle, rotation.ax, rotation.ay, rotation.az);
    return is;
}