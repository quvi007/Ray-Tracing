#include <iostream>

class Vector3D {
private:
    double x, y, z;
    
    friend istream &operator>>(istream &is, Vector3D &vector3D);
    friend ostream &operator<<(ostream &os, const Vector3D &vector3D);
    friend Vector3D operator*(double s, const Vector3D &vector3D);
public:
    Vector3D() {}
    Vector3D(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void setX(double x) {
        this->x = x;
    }

    void setY(double y) {
        this->y = y;
    }

    void setZ(double z) {
        this->z = z;
    }

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

    double getZ() const {
        return z;
    }

    Vector3D operator+(const Vector3D &obj) const {
        return Vector3D(x + obj.x, y + obj.y, z + obj.z);
    }

    Vector3D operator-(const Vector3D &obj) const {
        return Vector3D(x - obj.x, y - obj.y, z - obj.z);
    }

    Vector3D operator-() const {
        return Vector3D(-x, -y, -z);
    }

    Vector3D operator*(double s) const {
        return Vector3D(x * s, y * s, z * s);
    }

    static double dot(const Vector3D &a, const Vector3D &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
};

class Color {
private:
    friend istream &operator>>(istream &is, Color &color);
    friend ostream &operator<<(ostream &is, const Color &color);
public:
    double r, g, b;
};

class CoEfficients {
private:
    friend istream &operator>>(istream &is, CoEfficients &coEfficients);
    friend ostream &operator<<(ostream &os, const CoEfficients &coEfficients);
public:
    double ambient;
    double diffuse;
    double specular;
    double reflection;
};

class Object {
protected:
    Vector3D referencePoint; // should have x, y, z
    double height, width, length;
    Color color;
    CoEfficients coEfficients; // ambient, diffuse, specular, reflection coEfficients
    int shine; // exponent term of specular component
public:
    Object() {}

    virtual void draw() {}
    
    virtual double intersect(const Ray &ray, const Color &color, int level) {
        return -1.0;
    }

    void setReferencePoint(const Vector3D &referencePoint) {
        this->referencePoint = referencePoint;
    }

    void setColor(const Color &color) {
        this->color = color;
    }

    void setCoEfficients(const CoEfficients &coEfficients) {
        this->coEfficients = coEfficients;
    }
    
    void setShine(int shine) {
        this->shine = shine;
    }

    Vector3D getReferencePoint() const {
        return referencePoint;
    }

    Color getColor() const {
        return color;
    }

    CoEfficients getCoEfficients() const {
        return coEfficients;
    }

    int getShine() const {
        return shine;
    }
};

class Sphere : public Object {
public:
    Sphere() {}
    Sphere(const Vector3D &center, double radius) {
        this->referencePoint = center;
        this->length = radius;
    }
    
    void draw() {
        // write codes for drawing sphere
    }

    double intersect(const Ray &ray, const Color &color, int level) {
        Vector3D Ro = ray.getStart();
        Vector3D Rd = ray.getDir();
        Vector3D C = this->referencePoint;
        double r = this->length;

        double a, b, c;

        a = Vector3D::dot(Rd, Rd);
        b = 2 * (Vector3D::dot(Ro, Rd) - Vector3D::dot(Rd, C));
        c = Vector3D::dot(Ro, Ro) + Vector3D::dot(C, C) - 2 * Vector3D::dot(Ro, C) - r * r;

        double d = b * b - 4 * a * c;
        if (d < 0) return -1.0;
        d = sqrt(d);
        double t_p = ((-b + d) / (2 * a)), t_m = ((-b - d) / (2 * a));
        if (t_p < 0 && t_m < 0) return -1.0;
        if (t_p < 0) return t_m;
        if (t_m < 0) return t_p;
        return min(t_p, t_m);
    }
};

class Triangle : public Object {
public:
    Triangle() {}
    Triangle(const Vector3D &p1, const Vector3D &p2, const Vector3D &p3) {
        // what will be the triangle's constructor be like?
    }

    void draw() {
        // write codes for drawing triangle
    }

    double intersect(const Ray &ray, const Color &color, int level) {
        // implement
    }
};

class General : public Object {
public:
    General() {
        // write this constructor
    }

    void draw() {
        // write codes for drawing quad object
    }

    double intersect(const Ray &ray, const Color &color, int level) {
        // implement
    }
};

class PointLight {
private:
    Vector3D lightPos;
    Color color;

    friend istream &operator>>(istream &is, PointLight &pointLight);
    friend ostream &operator<<(ostream &os, const PointLight &pointLight);
public:
    PointLight() {}
    PointLight(const Vector3D &lightPos, const Color &color) {
        this->lightPos = lightPos;
        this->color = color;
    }

    void setLightPos(const Vector3D &lightPos) {
        this->lightPos = lightPos;
    }

    void setColor(const Color &color) {
        this->color = color;
    }

    Vector3D getLightPos() const {
        return lightPos;
    }

    Color getColor() const {
        return color;
    }
    
};

class SpotLight {
private:
    PointLight pointLight;
    Vector3D lightDirection;
    double cutoffAngle;

    friend istream &operator>>(istream &is, SpotLight &spotLight);
    friend ostream &operator<<(ostream &os, const SpotLight &spotLight);
public:
    SpotLight() {}
    SpotLight(const PointLight &pointLight, const Vector3D &lightDirection, double cutoffAngle) {
        this->pointLight = pointLight;
        this->lightDirection = lightDirection;
        this->cutoffAngle = cutoffAngle;
    }

    void setPointLight(const PointLight &pointLight) {
        this->pointLight = pointLight;
    }

    void setLightDirection(const Vector3D &lightDirection) {
        this->lightDirection = lightDirection;
    }

    void setCutoffAngle(double cutoffAngle) {
        this->cutoffAngle = cutoffAngle;
    }

    PointLight getPointLight() const {
        return pointLight;
    }

    Vector3D getLightDirection() const {
        return lightDirection;
    }

    double getCutoffAngle() const {
        return cutoffAngle;
    }

};

class Floor : public Object {
public: 
    Floor() {}
    Floor(double floorWidth, double tileWidth) {
        this->referencePoint = Vector3D(-floorWidth / 2, -floorWidth / 2, 0);
        this->length = tileWidth;
    }
    void draw() {
        // write code
    }
};

class Ray {
private:
    Vector3D start;
    Vector3D dir; // normalize for easier calculation

    friend istream &operator>>(istream &is, Ray &ray);
    friend ostream &operator<<(ostream &os, const Ray &ray);
public:
    Ray() {} 
    Ray(const Vector3D &start, const Vector3D &dir) {
        this->start = start;
        this->dir = dir;
    }

    void setStart(const Vector3D &start) {
        this->start = start;
    }

    void setDir(const Vector3D &dir) {
        this->dir = dir;
    }

    Vector3D getStart() const {
        return start;
    }

    Vector3D getDir() const {
        return dir;
    }
};

istream &operator>>(istream &is, Vector3D &vector3D) {
    is >> vector3D.x >> vector3D.y >> vector3D.z;
    return is;
}

ostream &operator<<(ostream &os, const Vector3D &vector3D) {
    os << vector3D.x << " " << vector3D.y << " " << vector3D.z;
    return os;
}

Vector3D operator*(double s, const Vector3D &vector3D) {
    return vector3D * s;
}

istream &operator>>(istream &is, Color &color) {
    is >> color.r >> color.g >> color.b;
    return is;
}

ostream &operator<<(ostream &os, const Color &color) {
    os << color.r << " " << color.g << " " << color.b;
    return os;
}

istream &operator>>(istream &is, CoEfficients &coEfficients) {
    is >> coEfficients.ambient >> coEfficients.diffuse >> coEfficients.specular >> coEfficients.reflection;
    return is;
}

ostream &operator<<(ostream &os, const CoEfficients &coEfficients) {
    os << coEfficients.ambient << " " << coEfficients.diffuse << " " << coEfficients.specular << " " << coEfficients.reflection;
    return os;
}

istream &operator>>(istream &is, PointLight &pointLight) {
    is >> pointLight.lightPos >> pointLight.color;
    return is;
}

ostream &operator<<(ostream &os, const PointLight &pointLight) {
    os << pointLight.lightPos << " " << pointLight.color;
    return os;
}

istream &operator>>(istream &is, SpotLight &spotLight) {
    is >> spotLight.pointLight >> spotLight.lightDirection >> spotLight.cutoffAngle;
    return is;
}

ostream &operator<<(ostream &os, const SpotLight &spotLight) {
    os << spotLight.pointLight << " " << spotLight.lightDirection << " " << spotLight.cutoffAngle;
    return os;
}

istream &operator>>(istream &is, Ray &ray) {
    is >> ray.start >> ray.dir;
    return is;
}

ostream &operator<<(ostream &os, const Ray &ray) {
    os << ray.start << " " << ray.dir;
    return os;
}

