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