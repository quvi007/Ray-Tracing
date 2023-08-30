class Vector3D {
public:
    double x, y, z;
};

class Object {
public:
    Vector3D reference_point; // should have x, y, z
    double height, width, length;
    double color[3];
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine; // exponent term of specular component
    Object() {}
    virtual void draw(){}
    void setColor(){}
    void setCoEfficients(){}
    void setShine(){}
};

class Sphere : Object {
    Sphere(const Vector3D &center, double radius) {
        this->reference_point = center;
        this->length = radius;
    }
    void draw() {
        // write codes for drawing sphere
    }
};

class Triangle : Object {
    Triangle(const Vector3D &p1, const Vector3D &p2, const Vector3D &p3) {
        // what will be the triangle's constructor be like?
    }
    void draw() {
        // write codes for drawing triangle
    }
};

class Quad : Object {

};

class PointLight {

};

class SpotLight {

};