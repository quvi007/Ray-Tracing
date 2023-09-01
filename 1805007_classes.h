#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>

#include "1805007_Rotation.h"
#include "1805007_Point.h"

#include "bitmap_image.hpp"

#define RAD(x) (x * M_PI / 180.0)

extern int level_of_recursion;

bool reflectionOn = true;

bool EQ(double a, double b) {
    if (abs(a - b) <= 1e-3) return true;
    return false;
}

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

    bool operator==(const Vector3D &obj) const {
        return x == obj.x && y == obj.y && z == obj.z;
    }

    bool operator!=(const Vector3D &obj) const {
        return x != obj.x || y != obj.y || z != obj.z;
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

bool triangleContainsPoint(const Vector3D &a, const Vector3D &b, const Vector3D &c, const Vector3D &p) {
    if (p == a || p == b || p == c) return true;
    double a1 = b.getX() - a.getX();
    double b1 = c.getX() - a.getX();
    double a2 = b.getY() - a.getY();
    double b2 = c.getY() - a.getY();
    double a3 = b.getZ() - a.getZ();
    double b3 = c.getZ() - a.getZ();
    double c1 = a.getX() - p.getX();
    double c2 = a.getY() - p.getY();
    double c3 = a.getZ() - p.getZ();

    double alpha, beta;

    if (a1 * b2 - a2 * b1 != 0) {
        alpha = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
        beta = (c1 * a2 - c2 * a1) / (a1 * b2 - a2 * b1);
    } else if (a1 * b3 - a3 * b1 != 0) {
        alpha = (b1 * c3 - b3 * c1) / (a1 * b3 - a3 * b1);
        beta = (c1 * a3 - c3 * a1) / (a1 * b3 - a3 * b1);
    } else if (a2 * b3 - a3 * b2 != 0) {
        alpha = (b2 * c3 - b3 * c2) / (a2 * b3 - a3 * b2);
        beta = (c2 * a3 - c3 * a2) / (a2 * b3 - a3 * b2);
    } else return false;

    if (alpha >= 0.0 && alpha <= 1.0 && beta >= 0.0 && beta <= 1.0 && (alpha + beta) <= 1.0) {
        return true;
    }
    return false;
}

bool rectangleContainsPoint(const Vector3D &a, const Vector3D &b, const Vector3D &c, const Vector3D &d, const Vector3D &p) {
    bool ans = triangleContainsPoint(a, b, d, p) || triangleContainsPoint(b, c, d, p);
    return ans;
}

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

    void clip() {
        if (r > 1) r = 1;
        if (r < 0) r = 0;

        if (g > 1) g = 1;
        if (g < 0) g = 0;

        if (b > 1) b = 1;
        if (b < 0) b = 0;
    }

    Color operator*(double k) const {
        return Color(r * k, g * k, b * k);
    }

    Color operator+(const Color &obj) const {
        double rr = r + obj.r;
        double gg = g + obj.g;
        double bb = b + obj.b;
        return Color(rr, gg, bb);
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

    void drawSphere(double radius, int numStacks, int numSlices) const {
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

    void draw() const {
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

    void drawSphere(double radius, int numStacks, int numSlices) const {
        glColor3d(0.8, 0.8, 0.0);
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

    void draw() const {
        Vector3D lightPos = pointLight.getLightPos();
        glPushMatrix();
        glTranslated(lightPos.getX(), lightPos.getY(), lightPos.getZ());
        drawSphere(5, 100, 100);
        glPopMatrix();
    }

};

extern vector<PointLight *> pointLights;
extern vector<SpotLight *> spotLights;

class Object {
protected:
    Color color;
    CoEfficients coEfficients; // ambient, diffuse, specular, reflection coEfficients
    int shine; // exponent term of specular component
public:
    string name;
    
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

extern vector<Object *> objects;
extern Vector3D eye;

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
        this->name = "sphere";
    }
    
    void draw() const {
        glPushMatrix();
        glTranslated(center.getX(), center.getY(), center.getZ());
        drawSphere(100, 100);
        glPopMatrix();
    }

    double intersect(const Ray &ray, Color &color, int level) const {
        if (level == 0) {
            color = Color(0, 0, 0);
            return -1.0;
        }

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

        color = Color(0, 0, 0);

        if (level == level_of_recursion + 1 || t < 0) return t;

        Vector3D intersectionPoint = ray.getStart() + t * ray.getDir();
        Color intersectionPointColor = this->color;
        color = intersectionPointColor;
        color = color * coEfficients.getAmbient();
        Vector3D normal = (intersectionPoint - center);
        normal.normalize();

        double lambert = 0, phong = 0;

        for (PointLight *pointLight : pointLights) {
            Vector3D toSource = pointLight->getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();
            Ray SP(pointLight->getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight->getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir() * (-1)), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
                color = color + intersectionPointColor * coEfficients.getSpecular() * phong;
            }
        }
        
        for (SpotLight *spotLight : spotLights) {
            PointLight pointLight = spotLight->getPointLight();
            Vector3D toSource = pointLight.getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();

            double angle = acos(Vector3D::dot(toSource * (-1), spotLight->getLightDirection()));

            if (angle > spotLight->getCutoffAngle()) continue;
            Ray SP(pointLight.getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight.getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir()), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
                color = color + intersectionPointColor * coEfficients.getSpecular() * phong;
            }
        }

        if (reflectionOn) {
            Vector3D toCamera = eye - intersectionPoint;
            toCamera.normalize();

            Vector3D reflectedVector = toCamera * (-1) + normal * (Vector3D::dot(toCamera, normal)) * 2;
            reflectedVector.normalize();

            Ray reflectedRay(intersectionPoint + 0.0001 * reflectedVector, reflectedVector);

            Color nextColor(0, 0, 0), nextDummyColor;
            double nextTMin = 1e9;
            Object *nextNearestObject = nullptr;
            for (Object *nextObject : objects) {
                double nextT = nextObject->intersect(reflectedRay, nextDummyColor, level_of_recursion + 1);
                if (nextT >= 0 && nextT < nextTMin) {
                    nextTMin = nextT;
                    nextNearestObject = nextObject;
                }
            }
            if (!nextNearestObject || nextTMin < 0 || nextTMin > 20000) return t;
            nextTMin = nextNearestObject->intersect(reflectedRay, nextColor, level - 1);

            color = color + nextColor * coEfficients.getReflection();
        }
        return t;
    }
};

class Triangle : public Object {
private:
    Vector3D a, b, c;
    void drawTriangle() const {
        glColor3d(color.getR(), color.getG(), color.getB());
        glBegin(GL_TRIANGLES);
            glVertex3d(a.getX(), a.getY(), a.getZ());
            glVertex3d(b.getX(), b.getY(), b.getZ());
            glVertex3d(c.getX(), c.getY(), c.getZ());
        glEnd();
    }   
public:
    Triangle() {}
    Triangle(const Vector3D &a, const Vector3D &b, const Vector3D &c) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->name = "triangle";
    }
    void draw() const {
        glPushMatrix();
        drawTriangle();
        glPopMatrix();
    }

    double intersect(const Ray &ray, Color &color, int level) const {
        if (level == 0) {
            color = Color(0, 0, 0);
            return -1.0;
        }

        double t = -1.0;

        Vector3D Ro = ray.getStart();
        Vector3D Rd = ray.getDir();

        Vector3D normal = (b - a) * (c - a);
        normal.normalize();

        t = (-(-Vector3D::dot(normal, a) + Vector3D::dot(normal, Ro))) / (Vector3D::dot(normal, Rd));
        color = Color(0, 0, 0);

        Vector3D intersectionPoint = ray.getStart() + t * ray.getDir();

        if (!triangleContainsPoint(a, b, c, intersectionPoint)) 
            t = -1.0;

        if (level == level_of_recursion + 1 || t < 0) return t;

        Color intersectionPointColor = this->color;
        color = intersectionPointColor;
        
        color = color * coEfficients.getAmbient();

        double lambert = 0, phong = 0;

        for (PointLight *pointLight : pointLights) {
            Vector3D toSource = pointLight->getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();
            Ray SP(pointLight->getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight->getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir() * (-1)), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
                color = color + intersectionPointColor * coEfficients.getSpecular() * phong;
            }
        }

        for (SpotLight *spotLight : spotLights) {
            PointLight pointLight = spotLight->getPointLight();
            Vector3D toSource = pointLight.getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();
 
            double angle = acos(Vector3D::dot(toSource * (-1), spotLight->getLightDirection()));

            if (angle > spotLight->getCutoffAngle()) continue;
            Ray SP(pointLight.getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight.getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir() * (-1)), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
                color = color + intersectionPointColor * coEfficients.getSpecular() * phong;
            }
        }

        if (reflectionOn) {
            Vector3D toCamera = eye - intersectionPoint;
            toCamera.normalize();

            Vector3D reflectedVector = toCamera * (-1) + normal * (Vector3D::dot(toCamera, normal)) * 2;
            reflectedVector.normalize();

            Ray reflectedRay(intersectionPoint + 0.0001 * reflectedVector, reflectedVector);

            Color nextColor(0, 0, 0), nextDummyColor;
            double nextTMin = 1e9;
            Object *nextNearestObject = nullptr;
            for (Object *nextObject : objects) {
                double nextT = nextObject->intersect(reflectedRay, nextDummyColor, level_of_recursion + 1);
                if (nextT >= 0 && nextT < nextTMin) {
                    nextTMin = nextT;
                    nextNearestObject = nextObject;
                }
            }
            if (!nextNearestObject || nextTMin < 0 || nextTMin > 20000) return t;
            nextTMin = nextNearestObject->intersect(reflectedRay, nextColor, level - 1);

            color = color + nextColor * coEfficients.getReflection();
        }

        return t;
    }
};

class Rectangle : public Object {
private:
    Vector3D a, b, c, d;
    void drawRectangle() const {
        glColor3d(color.getR(), color.getG(), color.getB());
        glBegin(GL_QUADS);
            glVertex3d(a.getX(), a.getY(), a.getZ());
            glVertex3d(b.getX(), b.getY(), b.getZ());
            glVertex3d(c.getX(), c.getY(), c.getZ());
            glVertex3d(d.getX(), d.getY(), d.getZ());
        glEnd();
    }
public:
    Rectangle() {}
    Rectangle(const Vector3D &a, const Vector3D &b, const Vector3D &c, const Vector3D &d) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->name = "rectangle";
    }
    void draw() const {
        glPushMatrix();
        drawRectangle();
        glPopMatrix();
    }
    
    double intersect(const Ray &ray, Color &color, int level) const {
        if (level == 0) {
            color = Color(0, 0, 0);
            return -1.0;
        }

        double t = -1.0;

        Vector3D Ro = ray.getStart();
        Vector3D Rd = ray.getDir();

        Vector3D normal = (b - a) * (d - a);
        normal.normalize();

        t = (-(-Vector3D::dot(normal, a) + Vector3D::dot(normal, Ro))) / (Vector3D::dot(normal, Rd));
        color = Color(0, 0, 0);

        Vector3D intersectionPoint = ray.getStart() + t * ray.getDir();

        if (!rectangleContainsPoint(a, b, c, d, intersectionPoint)) 
            t = -1.0;

        if (level == level_of_recursion + 1 || t < 0) return t;

        Color intersectionPointColor = this->color;
        color = intersectionPointColor;
        
        color = color * coEfficients.getAmbient();

        double lambert = 0, phong = 0;

        for (PointLight *pointLight : pointLights) {
            Vector3D toSource = pointLight->getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();
            Ray SP(pointLight->getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight->getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir() * (-1)), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
                color = color + intersectionPointColor * coEfficients.getSpecular() * phong;
            }
        }

        for (SpotLight *spotLight : spotLights) {
            PointLight pointLight = spotLight->getPointLight();
            Vector3D toSource = pointLight.getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();
 
            double angle = acos(Vector3D::dot(toSource * (-1), spotLight->getLightDirection()));

            if (angle > spotLight->getCutoffAngle()) continue;
            Ray SP(pointLight.getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight.getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir() * (-1)), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
                color = color + intersectionPointColor * coEfficients.getSpecular() * phong;
            }
        }
        
        if (reflectionOn) {
            Vector3D toCamera = eye - intersectionPoint;
            toCamera.normalize();

            Vector3D reflectedVector = toCamera * (-1) + normal * (Vector3D::dot(toCamera, normal)) * 2;
            reflectedVector.normalize();

            Ray reflectedRay(intersectionPoint + 2 * reflectedVector, reflectedVector);

            Color nextColor(0, 0, 0), nextDummyColor;
            double nextTMin = 1e9;
            Object *nextNearestObject = nullptr;
            for (Object *nextObject : objects) {
                double nextT = nextObject->intersect(reflectedRay, nextDummyColor, level_of_recursion + 1);
                if (nextT >= 0 && nextT < nextTMin) {
                    nextTMin = nextT;
                    nextNearestObject = nextObject;
                }
            }
            if (!nextNearestObject || nextTMin < 0 || nextTMin > 20000) return t;
            nextTMin = nextNearestObject->intersect(reflectedRay, nextColor, level - 1);

            color = color + nextColor * coEfficients.getReflection();
        }
        return t;
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
                    glColor3f(1.0f, 1.0f, 1.0f);
                } else {
                    glColor3f(0.0f, 0.0f, 0.0f);
                }
                glVertex3f(j * tileWidth, i * tileWidth, 0.0f);
                glVertex3f((j + 1) * tileWidth, i * tileWidth, 0.0f);
                glVertex3f((j + 1) * tileWidth, (i + 1) * tileWidth, 0.0f);
                glVertex3f(j * tileWidth, (i + 1) * tileWidth, 0.0f);
                glEnd();
                
                isWhite = !isWhite;
            }
            if (count % 2 == 0) {
                isWhite = !isWhite;
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
        if (level == 0) {
            color = Color(0, 0, 0);
            return -1.0;
        }

        double t = -1.0;

        Vector3D Ro = ray.getStart();
        Vector3D Rd = ray.getDir();

        Vector3D normal(0, 0, 1);

        t = (-Vector3D::dot(normal, Ro)) / (Vector3D::dot(normal, Rd));
        
        color = Color(0, 0, 0);

        if (level == level_of_recursion + 1 || t < 0) return t;

        Vector3D intersectionPoint = ray.getStart() + t * ray.getDir();

        Vector3D topLeftPoint(-floorWidth / 2, floorWidth / 2, 0);

        Vector3D diff = intersectionPoint - topLeftPoint;

        int dx = ceil(abs(diff.getX()) / tileWidth);
        int dy = ceil(abs(diff.getY()) / tileWidth);

        Color tempColor;

        if ((dx + dy) % 2) tempColor = Color(1, 1, 1);
        else tempColor = Color(0, 0, 0);

        Color intersectionPointColor = tempColor;
        color = intersectionPointColor;
        
        color = color * coEfficients.getAmbient();

        double lambert = 0, phong = 0;

        for (PointLight *pointLight : pointLights) {
            Vector3D toSource = pointLight->getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();
            Ray SP(pointLight->getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight->getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir() * (-1)), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
            }
        }

        for (SpotLight *spotLight : spotLights) {
            PointLight pointLight = spotLight->getPointLight();
            Vector3D toSource = pointLight.getLightPos() - intersectionPoint;
            double distance = toSource.magnitude();
            toSource.normalize();
 
            double angle = acos(Vector3D::dot(toSource * (-1), spotLight->getLightDirection()));

            if (angle > spotLight->getCutoffAngle()) continue;
            Ray SP(pointLight.getLightPos(), toSource * (-1));
            Color dummyColor(1, 1, 1);
            double tMin = 1e9;
            for (Object *object : objects) {
                double t = object->intersect(SP, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                }
            }
            if (EQ(tMin, distance)) {
                double scaling_factor = exp(- distance * distance * pointLight.getFalloffParameter());
                lambert += Vector3D::dot(toSource, normal) * scaling_factor;
                Vector3D L = toSource;
                Vector3D R = L * (-1) + normal * (Vector3D::dot(L, normal)) * 2;
                phong += pow(Vector3D::dot(R, ray.getDir() * (-1)), shine) * scaling_factor;
                color = color + intersectionPointColor * coEfficients.getDiffuse() * lambert;
            }
        }

        if (reflectionOn) {
            Vector3D toCamera = eye - intersectionPoint;
            toCamera.normalize();

            Vector3D reflectedVector = toCamera * (-1) + normal * (Vector3D::dot(toCamera, normal)) * 2;
            reflectedVector.normalize();

            Ray reflectedRay(intersectionPoint + 0.0001 * reflectedVector, reflectedVector);

            Color nextColor(0, 0, 0), nextDummyColor;
            double nextTMin = 1e9;
            Object *nextNearestObject = nullptr;
            for (Object *nextObject : objects) {
                double nextT = nextObject->intersect(reflectedRay, nextDummyColor, level_of_recursion + 1);
                if (nextT >= 0 && nextT < nextTMin) {
                    nextTMin = nextT;
                    nextNearestObject = nextObject;
                }
            }
            if (!nextNearestObject || nextTMin < 0 || nextTMin > 20000) return t;
            nextTMin = nextNearestObject->intersect(reflectedRay, nextColor, level - 1);

            color = color + nextColor * coEfficients.getReflection();
        }   
        return t;
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

