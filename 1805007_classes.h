#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>

#include "Rotation.h"
#include "Point.h"

#include "bitmap_image.hpp"

#define RAD(x) (x * M_PI / 180.0)

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

    Vector3D operator*(const Vector3D &obj) const {
        return Vector3D(y * obj.z - z * obj.y, z * obj.x - x * obj.z, x * obj.y - y * obj.x);
    }

    Vector3D operator*(double s) const {
        return Vector3D(x * s, y * s, z * s);
    }

    static double dot(const Vector3D &a, const Vector3D &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    void normalize() {
        double magnitude = this->magnitude();
        if (magnitude != 0) *this = *this * (1 / magnitude);
    }

    double magnitude() const {
        return sqrt(Vector3D::dot(*this, *this));
    }
};

class Color {
private:
    double r, g, b;
    
    friend istream &operator>>(istream &is, Color &color);
    friend ostream &operator<<(ostream &is, const Color &color);
public:
    Color() {}
    Color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void setR(double r) {
        this->r = r;
    }

    void setG(double g) {
        this->g = g;
    }

    void setB(double b) {
        this->b = b;
    }

    double getR() const {
        return r;
    }

    double getG() const {
        return g;
    }

    double getB() const {
        return b;
    }

    Color operator*(double k) const {
        return Color(r * k, g * k, b * k);
    }

    Color operator+(const Color &obj) const {
        return Color(r + obj.r, g + obj.g, b + obj.b);
    }
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
    
    CoEfficients() {}
    CoEfficients(double ambient, double diffuse, double specular, double reflection) {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
    }

    void setAmbient(double ambient) {
        this->ambient = ambient;
    }

    void setDiffuse(double diffuse) {
        this->diffuse = diffuse;
    }

    void setSpecular(double specular) {
        this->specular = specular;
    }

    void setReflection(double reflection) {
        this->reflection = reflection;
    }

    double getAmbient() const {
        return ambient;
    }

    double getDiffuse() const {
        return diffuse;
    }

    double getSpecular() const {
        return specular;
    }

    double getReflection() const {
        return reflection;
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
        double mag = sqrt(Vector3D::dot(dir, dir));
        if (mag > 0)
            this->dir = this->dir * (1/mag);
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


class PointLight {
private:
    Vector3D lightPos;
    double falloffParameter;

    friend istream &operator>>(istream &is, PointLight &pointLight);
    friend ostream &operator<<(ostream &os, const PointLight &pointLight);

    void drawSphere(double radius, int numStacks, int numSlices) {
        glColor3d(0.8, 0.8, 0.8);
        for (int i = 0; i < numStacks; ++i) {
            float phi1 = static_cast<float>(M_PI) * static_cast<float>(i) / numStacks;
            float phi2 = static_cast<float>(M_PI) * static_cast<float>(i + 1) / numStacks;
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= numSlices; ++j) {
                float theta = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j) / numSlices;
                
                float x1 = radius * sin(phi1) * cos(theta);
                float y1 = radius * cos(phi1);
                float z1 = radius * sin(phi1) * sin(theta);
                
                float x2 = radius * sin(phi2) * cos(theta);
                float y2 = radius * cos(phi2);
                float z2 = radius * sin(phi2) * sin(theta);
                
                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
            }
            glEnd();
        }
    }
public:
    PointLight() {}
    PointLight(const Vector3D &lightPos, double falloffParameter) {
        this->lightPos = lightPos;
        this->falloffParameter = falloffParameter;
    }

    void draw() {
        glPushMatrix();
        glTranslated(lightPos.getX(), lightPos.getY(), lightPos.getZ());
        drawSphere(5, 100, 100);
        glPopMatrix();
    }

    void setLightPos(const Vector3D &lightPos) {
        this->lightPos = lightPos;
    }

    Vector3D getLightPos() const {
        return lightPos;
    }

    void setFalloffParameter(double falloffParameter) {
        this->falloffParameter = falloffParameter;
    }

    double getFalloffParameter() const {
        return falloffParameter;
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

extern vector<PointLight *> pointLights;

class Object {
protected:
    Color color;
    CoEfficients coEfficients; // ambient, diffuse, specular, reflection coEfficients
    int shine; // exponent term of specular component
public:
    Object() {}

    virtual void draw() const {}
    
    virtual double intersect(const Ray &ray, Color &color, int level) const {
        return -1.0;
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
private:
    Vector3D center;
    double radius;
    void drawSphere(int numStacks, int numSlices) const {
        glColor3d(color.getR(), color.getG(), color.getB());
        for (int i = 0; i < numStacks; ++i) {
            float phi1 = static_cast<float>(M_PI) * static_cast<float>(i) / numStacks;
            float phi2 = static_cast<float>(M_PI) * static_cast<float>(i + 1) / numStacks;
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= numSlices; ++j) {
                float theta = 2.0f * static_cast<float>(M_PI) * static_cast<float>(j) / numSlices;
                
                float x1 = radius * sin(phi1) * cos(theta);
                float y1 = radius * cos(phi1);
                float z1 = radius * sin(phi1) * sin(theta);
                
                float x2 = radius * sin(phi2) * cos(theta);
                float y2 = radius * cos(phi2);
                float z2 = radius * sin(phi2) * sin(theta);
                
                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
            }
            glEnd();
        }
    }
public:
    Sphere() {}
    Sphere(const Vector3D &center, double radius) {
        this->center = center;
        this->radius = radius;
    }
    
    void draw() const {
        glPushMatrix();
        glTranslated(center.getX(), center.getY(), center.getZ());
        drawSphere(100, 100);
        glPopMatrix();
    }

    double intersect(const Ray &ray, Color &color, int level) const {
        double t = -1.0;
        
        Vector3D Ro = ray.getStart();
        Vector3D Rd = ray.getDir();
        Vector3D C = center;
        double r = radius;

        double a, b, c;

        a = Vector3D::dot(Rd, Rd);
        b = 2 * (Vector3D::dot(Ro, Rd) - Vector3D::dot(Rd, C));
        c = Vector3D::dot(Ro, Ro) + Vector3D::dot(C, C) - 2 * Vector3D::dot(Ro, C) - r * r;

        double d = b * b - 4 * a * c;
        if (d < 0) {
            t = -1.0;
        } else {
            d = sqrt(d);
            double t_p = ((-b + d) / (2 * a)), t_m = ((-b - d) / (2 * a));
            if (t_p < 0 && t_m < 0) t = -1.0;
            else if (t_p < 0) t = t_m;
            else if (t_m < 0) t = t_p;
            else t = min(t_p, t_m);
        }

        if (level == 0) return t;

        Vector3D intersectionPoint = ray.getStart() + t * ray.getDir();

        Color intersectionPointColor = this->color;
        // color = intersectionPointColor * coEfficients.getAmbient();
        // Vector3D normal = (intersectionPoint - center);
        // normal.normalize();

        // double lambert = 0, phong = 0;

        // for (PointLight *pointLight : pointLights) {
        //     Vector3D toSource = intersectionPoint - pointLight->getLightPos();
        //     double distance = toSource.magnitude();
        //     toSource.normalize();
        //     double scaling_factor = exp(- distance * distance * pointLight->getFalloffParameter());
        //     lambert = Vector3D::dot(toSource, normal) * scaling_factor;
        //     color = color + Color(1, 1, 1) * coEfficients.getDiffuse() * lambert;
        // }
        
        // if (level >= level_of_recursion) return t;

        return t;

    }
};

class Pyramid : public Object {
private:
    Vector3D lowest_point;
    double width;
    double height;

    void drawPyramid() const {
        glColor3d(color.getR(), color.getG(), color.getB());
        glBegin(GL_QUADS);
            glVertex3d(0, 0, 0);
            glVertex3d(width, 0, 0);
            glVertex3d(width, width, 0);
            glVertex3d(0, width, 0);
        glEnd();
        glBegin(GL_TRIANGLES);
            glVertex3d(0, 0, 0);
            glVertex3d(width, 0, 0);
            glVertex3d(width / 2, width / 2, height);

            glVertex3d(width, 0, 0);
            glVertex3d(width, width, 0);
            glVertex3d(width / 2, width / 2, height);

            glVertex3d(width, width, 0);
            glVertex3d(0, width, 0);
            glVertex3d(width / 2, width / 2, height);

            glVertex3d(0, width, 0);
            glVertex3d(0, 0, 0);
            glVertex3d(width / 2, width / 2, height);
        glEnd();
    }
public:
    Pyramid() {}
    Pyramid(const Vector3D &lowest_point, double width, double height) {
        this->lowest_point = lowest_point;
        this->width = width;
        this->height = height;
    }

    void draw() const {
        glPushMatrix();
        glTranslated(lowest_point.getX(), lowest_point.getY(), lowest_point.getZ());
        drawPyramid();
        glPopMatrix();
    }

    double intersect(const Ray &ray, Color &color, int level) const {
        // implement
        return -1.0;
    }
};

class Cube : public Object {
private:
    Vector3D bottom_lower_left_point;
    double side;

    void drawCube() const {
        glColor3d(color.getR(), color.getG(), color.getB());
        glBegin(GL_QUADS);
            glVertex3d(0, 0, 0);
            glVertex3d(side, 0, 0);
            glVertex3d(side, side, 0);
            glVertex3d(0, side, 0);

            glVertex3d(0, 0, 0);
            glVertex3d(side, 0, 0);
            glVertex3d(side, 0, side);
            glVertex3d(0, 0, side);

            glVertex3d(side, 0, 0);
            glVertex3d(side, side, 0);
            glVertex3d(side, side, side);
            glVertex3d(side, 0, side);
            
            glVertex3d(side, side, 0);
            glVertex3d(0, side, 0);
            glVertex3d(0, side, side);
            glVertex3d(side, side, side);
            
            glVertex3d(0, side, 0);
            glVertex3d(0, 0, 0);
            glVertex3d(0, 0, side);
            glVertex3d(0, side, side);
            
            glVertex3d(0, 0, side);
            glVertex3d(side, 0, side);
            glVertex3d(side, side, side);
            glVertex3d(0, side, side);
        glEnd();
    }
public:
    Cube() {}
    Cube(const Vector3D &bottom_lower_left_point, double side) {
        this->bottom_lower_left_point = bottom_lower_left_point;
        this->side = side;
    }

    void draw() const {
        glPushMatrix();
        glTranslated(bottom_lower_left_point.getX(), bottom_lower_left_point.getY(), bottom_lower_left_point.getZ());
        drawCube();
        glPopMatrix();
    }

    double intersect(const Ray &ray, Color &color, int level) const {
        // implement
        return -1.0;
    }
};

class Floor : public Object {
private:
    int floorWidth;
    int tileWidth;

    void drawCheckerBoard(int floorWidth, int tileWidth) const {
        int count = floorWidth / tileWidth;
        bool isWhite = true;
        for (int i = 0; i < count; ++i) {
            for (int j = 0; j < count; ++j) {
                glBegin(GL_QUADS);
                if (isWhite) {
                    glColor3f(1.0f, 1.0f, 1.0f); // White color
                } else {
                    glColor3f(0.0f, 0.0f, 0.0f); // Black color
                }
                glVertex3f(j * tileWidth, i * tileWidth, 0.0f);
                glVertex3f((j + 1) * tileWidth, i * tileWidth, 0.0f);
                glVertex3f((j + 1) * tileWidth, (i + 1) * tileWidth, 0.0f);
                glVertex3f(j * tileWidth, (i + 1) * tileWidth, 0.0f);
                glEnd();
                
                isWhite = !isWhite; // Switch color for the next tile
            }
            if (count % 2 == 0) {
                isWhite = !isWhite; // Offset the color for each row
            }
        }
    }
public: 
    Floor() {}
    Floor(double floorWidth, double tileWidth) {
        this->floorWidth = (int) floorWidth;
        this->tileWidth = (int) tileWidth;
    }
    void draw() const {
        glPushMatrix();
        glTranslated(-floorWidth / 2, -floorWidth / 2, 0);
        drawCheckerBoard(floorWidth, tileWidth);
        glPopMatrix();
    }

    double intersect(const Ray &ray, Color &color, int level) const {
        return -1.0;
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
    is >> pointLight.lightPos >> pointLight.falloffParameter;
    return is;
}

ostream &operator<<(ostream &os, const PointLight &pointLight) {
    os << pointLight.lightPos << " " << pointLight.falloffParameter;
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

