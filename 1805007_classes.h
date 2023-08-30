#include <bits/stdc++.h>

class Vector3D {
private:
    friend istream &operator>>(istream &is, Vector3D &vector3D);
    friend ostream &operator<<(ostream &os, const Vector3D &vector3D);
public:
    double x, y, z;
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
    Sphere(const Vector3D &center, double radius) {
        this->referencePoint = center;
        this->length = radius;
    }
    void draw() {
        // write codes for drawing sphere
    }
};

class Triangle : public Object {
public:
    Triangle(const Vector3D &p1, const Vector3D &p2, const Vector3D &p3) {
        // what will be the triangle's constructor be like?
    }
    void draw() {
        // write codes for drawing triangle
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

istream &operator>>(istream &is, Vector3D &vector3D) {
    is >> vector3D.x >> vector3D.y >> vector3D.z;
    return is;
}

ostream &operator<<(ostream &os, const Vector3D &vector3D) {
    os << vector3D.x << " " << vector3D.y << " " << vector3D.z;
    return os;
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
}

