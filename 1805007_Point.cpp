#include "1805007_Point.h"
#include <cmath>

Point::Point() : Point(0, 0, 0) {}

Point::Point(double px, double py, double pz) : Point(px, py, pz, 1) {}

Point::Point(double px, double py, double pz, double w) : px{px}, py{py}, pz{pz}, w{w} {
    double p[][1] = {{px}, {py}, {pz}, {1}};
    matrix = Matrix(4, 1, (const double *) p);
}

Point::Point(const Point &point) : Point(point.px, point.py, point.pz, point.w) {}

Point::Point(const Matrix &matrix) {
    this->matrix = matrix;
    px = this->matrix[0][0];
    py = this->matrix[1][0];
    pz = this->matrix[2][0];
    w = this->matrix[3][0];
}

Point &Point::operator=(const Point &rhs) {
    px = rhs.px;
    py = rhs.py;
    pz = rhs.pz;
    w = rhs.w;
    matrix = rhs.matrix;
    return *this;
}

Point Point::operator+(const Point &obj) const {
    return Point(matrix + obj.getMatrix());
}
Point Point::operator-(const Point &obj) const {
    return Point(matrix - obj.getMatrix());
}
Point Point::operator*(double s) const {
    return Point(matrix * s);
}

double Point::getPx() const {
    return px;
}

double Point::getPy() const {
    return py;
}

double Point::getPz() const {
    return pz;
}

double Point::getW() const {
    return w;
}

Matrix Point::getMatrix() const {
    return matrix;
}

void Point::setPx(double px) {
    this->px = matrix[0][0] = px;
}

void Point::setPy(double py) {
    this->py = matrix[1][0] = py;
}

void Point::setPz(double pz) {
    this->pz = matrix[2][0] = pz;
}

void Point::setW(double w) {
    this->w = matrix[3][0] = w;
}

Point Point::transform(const Matrix &matrix) const {
    return Point(matrix * this->matrix).scaleDown();
}

void Point::normalize() {
    double magnitude = sqrt(px * px + py * py + pz * pz);
    if (magnitude == 0) return;
    setPx(px / magnitude);
    setPy(py / magnitude);
    setPz(pz / magnitude);
}

Point Point::scaleDown() const {
    if (w > 1) {
        return Point(px / w, py / w, pz / w);
    } else return (*this);
}

Point Point::operator*(const Point &obj) const {
    double m[][4] = {{0, -pz, py, 0}, {pz, 0, -px, 0}, {-py, px, 0, 0}, {0, 0, 0, 1}};
    Matrix ans = Matrix(4, 4, (const double *) m) * obj.getMatrix();
    return Point(ans);
}

double Point::dot(const Point &a, const Point &b) {
    return a.px * b.px + a.py * b.py + a.pz * b.pz;
}

Point operator*(double s, const Point &point) {
    return Point(s * point.matrix);
}

ostream &operator<<(ostream &os, const Point &point) {
    os << fixed << setprecision(7) << point.px << " " << point.py << " " << point.pz;
    return os;
}

istream &operator>>(istream &is, Point &point) {
    is >> point.px >> point.py >> point.pz;
    point.matrix[0][0] = point.px;
    point.matrix[1][0] = point.py;
    point.matrix[2][0] = point.pz;
    point.matrix[3][0] = 1;
    return is;
}
