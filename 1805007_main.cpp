#include <bits/stdc++.h>

using namespace std;

#include "1805007_classes.h"

string scene_file = "scene.txt";

double nearDistance, farDistance;
double fovY;
double aspectRatio;

int level_of_recursion;
int dimension;

int number_of_objects;
int number_of_point_light_sources;
int number_of_spot_light_sources;
// declaration

vector<Object *> objects;
vector<PointLight *> pointLights;
vector<SpotLight *> spotLights;

void loadData() {
    ifstream ifs(scene_file);

    ifs >> nearDistance >> farDistance >> fovY >> aspectRatio;
    
    ifs >> level_of_recursion;
    ifs >> dimension;

    double tileWidth;
    double floorAmbientCoEff, floorDiffuseCoEff, floorReflectionCoEff;
    ifs >> tileWidth >> floorAmbientCoEff >> floorDiffuseCoEff >> floorReflectionCoEff;
    Object *floor = new Floor(20000, tileWidth);
    floor->setCoEfficients(CoEfficients(floorAmbientCoEff, floorDiffuseCoEff, 0, floorReflectionCoEff));
    objects.push_back(floor);
    
    ifs >> number_of_objects;
    for (int i = 0; i < number_of_objects; ++i) {
        string obj_type;
        ifs >> obj_type;
        if (obj_type == "sphere") {
            Vector3D center;
            double radius;
            Color color;
            CoEfficients coEfficients;
            int shine;

            ifs >> center >> radius >> color >> coEfficients >> shine;

            Object *sphere = new Sphere(center, radius);

            sphere->setColor(color);
            sphere->setCoEfficients(coEfficients);
            sphere->setShine(shine);

            objects.push_back(sphere);
        } else if (obj_type == "pyramid") {
            Vector3D lowest_point;
            double width;
            double height;
            Color color;
            CoEfficients coEfficients;
            int shine;
            ifs >> lowest_point >> width >> height >> color >> coEfficients >> shine;

            Vector3D a(0, 0, 0);
            Vector3D b(width, 0, 0);
            Vector3D c(width, width, 0);
            Vector3D d(0, width, 0);
            Vector3D e(width / 2, width / 2, height);

            a = a + lowest_point;
            b = b + lowest_point;
            c = c + lowest_point;
            d = d + lowest_point;
            e = e + lowest_point;

            Object *rectangle = new Rectangle(a, b, c, d);
            rectangle->setColor(color);
            rectangle->setCoEfficients(coEfficients);
            rectangle->setShine(shine);
            objects.push_back(rectangle);

            Object *triangle = new Triangle(a, b, e);
            triangle->setColor(color);
            triangle->setCoEfficients(coEfficients);
            triangle->setShine(shine);
            objects.push_back(triangle);

            triangle = new Triangle(b, c, e);
            triangle->setColor(color);
            triangle->setCoEfficients(coEfficients);
            triangle->setShine(shine);
            objects.push_back(triangle);

            triangle = new Triangle(c, d, e);
            triangle->setColor(color);
            triangle->setCoEfficients(coEfficients);
            triangle->setShine(shine);
            objects.push_back(triangle);

            triangle = new Triangle(d, a, e);
            triangle->setColor(color);
            triangle->setCoEfficients(coEfficients);
            triangle->setShine(shine);
            objects.push_back(triangle);
        } else if (obj_type == "cube") {
            Vector3D bottom_lower_left_point;
            double side;
            Color color;
            CoEfficients coEfficients;
            int shine;
            ifs >> bottom_lower_left_point >> side >> color >> coEfficients >> shine;

            Vector3D a, b, c, d;
            Object *rectangle;

            // 1
            a = Vector3D(0, 0, 0);
            b = Vector3D(side, 0, 0);
            c = Vector3D(side, side, 0);
            d = Vector3D(0, side, 0);

            a = a + bottom_lower_left_point;
            b = b + bottom_lower_left_point;
            c = c + bottom_lower_left_point;
            d = d + bottom_lower_left_point;

            rectangle = new Rectangle(a, b, c, d);
            rectangle->setColor(color);
            rectangle->setCoEfficients(coEfficients);
            rectangle->setShine(shine);
            objects.push_back(rectangle);

            // 2
            a = Vector3D(0, 0, 0);
            b = Vector3D(side, 0, 0);
            c = Vector3D(side, 0, side);
            d = Vector3D(0, 0, side);

            a = a + bottom_lower_left_point;
            b = b + bottom_lower_left_point;
            c = c + bottom_lower_left_point;
            d = d + bottom_lower_left_point;

            rectangle = new Rectangle(a, b, c, d);
            rectangle->setColor(color);
            rectangle->setCoEfficients(coEfficients);
            rectangle->setShine(shine);
            objects.push_back(rectangle);

            // 3
            a = Vector3D(side, 0, 0);
            b = Vector3D(side, side, 0);
            c = Vector3D(side, side, side);
            d = Vector3D(side, 0, side);

            a = a + bottom_lower_left_point;
            b = b + bottom_lower_left_point;
            c = c + bottom_lower_left_point;
            d = d + bottom_lower_left_point;

            rectangle = new Rectangle(a, b, c, d);
            rectangle->setColor(color);
            rectangle->setCoEfficients(coEfficients);
            rectangle->setShine(shine);
            objects.push_back(rectangle);

            // 4
            a = Vector3D(side, side, 0);
            b = Vector3D(0, side, 0);
            c = Vector3D(0, side, side);
            d = Vector3D(side, side, side);

            a = a + bottom_lower_left_point;
            b = b + bottom_lower_left_point;
            c = c + bottom_lower_left_point;
            d = d + bottom_lower_left_point;

            rectangle = new Rectangle(a, b, c, d);
            rectangle->setColor(color);
            rectangle->setCoEfficients(coEfficients);
            rectangle->setShine(shine);
            objects.push_back(rectangle);

            // 5
            a = Vector3D(0, side, 0);
            b = Vector3D(0, 0, 0);
            c = Vector3D(0, 0, side);
            d = Vector3D(0, side, side);

            a = a + bottom_lower_left_point;
            b = b + bottom_lower_left_point;
            c = c + bottom_lower_left_point;
            d = d + bottom_lower_left_point;

            rectangle = new Rectangle(a, b, c, d);
            rectangle->setColor(color);
            rectangle->setCoEfficients(coEfficients);
            rectangle->setShine(shine);
            objects.push_back(rectangle);

            // 6
            a = Vector3D(0, 0, side);
            b = Vector3D(side, 0, side);
            c = Vector3D(side, side, side);
            d = Vector3D(0, side, side);

            a = a + bottom_lower_left_point;
            b = b + bottom_lower_left_point;
            c = c + bottom_lower_left_point;
            d = d + bottom_lower_left_point;

            rectangle = new Rectangle(a, b, c, d);
            rectangle->setColor(color);
            rectangle->setCoEfficients(coEfficients);
            rectangle->setShine(shine);
            objects.push_back(rectangle);
        }
    }

    ifs >> number_of_point_light_sources;
    for (int i = 0; i < number_of_point_light_sources; ++i) {
        Vector3D lightPos;
        double falloffParameter;
        ifs >> lightPos >> falloffParameter;

        PointLight *pointLight = new PointLight(lightPos, falloffParameter);
        pointLights.push_back(pointLight);
    }

    ifs >> number_of_spot_light_sources;
    for (int i = 0; i < number_of_spot_light_sources; ++i) {
        PointLight pointLight;
        Vector3D lookingAt;
        double cutoffAngle;

        ifs >> pointLight >> lookingAt >> cutoffAngle;

        Vector3D lightDirection = lookingAt - pointLight.getLightPos();
        lightDirection.normalize();
        cutoffAngle = RAD(cutoffAngle);
        SpotLight *spotLight = new SpotLight(pointLight, lightDirection, cutoffAngle);
        spotLights.push_back(spotLight);
    }

    ifs.close(); 
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

int fileCount = 0;

bool textureOn = false;

Vector3D eye(40, -100, 40);
Vector3D center(0, 0, 0);
Vector3D u(0, 0, 1);
Vector3D l, r;

void calculateLR() {
    l = center - eye;
    l.normalize();
    r = l * u;
    r.normalize();
}

vector<vector<Color>> w_tex(256, vector<Color>(256, Color(1, 1, 1)));
vector<vector<Color>> b_tex(256, vector<Color>(256, Color(0, 0, 0)));

void loadTexture() {
    bitmap_image w_tex_bmp("texture_w.bmp"), b_tex_bmp("texture_b.bmp");

    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            bitmap_image::rgb_t colW = w_tex_bmp.get_pixel(j, i);
            bitmap_image::rgb_t colB = b_tex_bmp.get_pixel(j, i);
            w_tex[i][j] = Color(colW.red / 255.0, colW.green / 255.0, colW.blue / 255.0);
            b_tex[i][j] = Color(colB.red / 255.0, colB.green / 255.0, colB.blue / 255.0);
        }
    }
}

void capture() {
    double fovX = aspectRatio * fovY;
    calculateLR();

    bitmap_image bmp(dimension, dimension);
    double windowHeight = 2 * nearDistance * tan(RAD(fovY / 2));
    double windowWidth = 2 * nearDistance * tan(RAD(fovX / 2));
    Vector3D topLeft = eye + l * nearDistance - r * (windowWidth / 2) + u * (windowHeight / 2);
    double du = windowWidth / dimension;
    double dv = windowHeight / dimension;

    topLeft = topLeft + r * (0.5 * du) - u * (0.5 * dv);

    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j < dimension; ++j) {
            Vector3D curPixel = topLeft + i * du * r - j * dv * u;
            Vector3D dirr = (curPixel - eye);
            dirr.normalize();
            Ray ray(curPixel, dirr);
            Color color(0, 0, 0), dummyColor;
            double tMin = 1e9;
            Object *nearestObject = nullptr;

            for (Object * object : objects) {
                double t = object->intersect(ray, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                    nearestObject = object;
                }
            }
            if (!nearestObject || tMin < 0 || tMin > 4000) continue;
            tMin = nearestObject->intersect(ray, color, level_of_recursion);
            color.clip();
            bmp.set_pixel(i, j, color.getR() * 255, color.getG() * 255, color.getB() * 255);
        }
    }

    bmp.save_image("out-" + to_string(fileCount) + ".bmp");
    fileCount++;
}

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(600,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,600,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,600);
    glEnd();
}

/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(eye.getX(), eye.getY(), eye.getZ(),
            center.getX(), center.getY(), center.getZ(),
            u.getX(), u.getY(), u.getZ());
    // draw
    drawAxes();

    for (Object *object : objects) {
        object->draw();
    }

    for (PointLight *pointLight : pointLights) {
        pointLight->draw();
    }

    for (SpotLight *spotLight : spotLights) {
        spotLight->draw();
    }

    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(fovY, aspectRatio, nearDistance, farDistance);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    double speed = 5;
    Rotation rotation;

    calculateLR();

    Point temp;

    switch (key) {
        case 32:
            textureOn = !textureOn;
            break;
        case '0':
            capture();
            break;
        case 'a':
            rotation = Rotation(-speed, u.getX(), u.getY(), u.getZ());
            eye = eye - center;
            temp = (Point(eye.getX(), eye.getY(), eye.getZ())).transform(rotation.getMatrix());
            eye = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            eye = eye + center;
            break;
        case 'd':
            rotation = Rotation(speed, u.getX(), u.getY(), u.getZ());
            eye = eye - center;
            temp = (Point(eye.getX(), eye.getY(), eye.getZ())).transform(rotation.getMatrix());
            eye = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            eye = eye + center;
            break;
        case '1':
            rotation = Rotation(speed, u.getX(), u.getY(), u.getZ());
            center = center - eye;
            temp = (Point(center.getX(), center.getY(), center.getZ())).transform(rotation.getMatrix());
            center = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            center = center + eye;
            break;
        case '2':
            rotation = Rotation(-speed, u.getX(), u.getY(), u.getZ());
            center = center - eye;
            temp = (Point(center.getX(), center.getY(), center.getZ())).transform(rotation.getMatrix());
            center = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            center = center + eye;
            break;
        case '3':
            rotation = Rotation(speed, r.getX(), r.getY(), r.getZ());
            center = center - eye;
            temp = (Point(center.getX(), center.getY(), center.getZ())).transform(rotation.getMatrix());
            center = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            center = center + eye;
            
            temp = (Point(u.getX(), u.getY(), u.getZ())).transform(rotation.getMatrix());
            u = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            break;
        case '4':
            rotation = Rotation(-speed, r.getX(), r.getY(), r.getZ());
            center = center - eye;
            temp = (Point(center.getX(), center.getY(), center.getZ())).transform(rotation.getMatrix());
            center = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            center = center + eye;

            temp = (Point(u.getX(), u.getY(), u.getZ())).transform(rotation.getMatrix());
            u = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            break;
        case '5':
            rotation = Rotation(speed, l.getX(), l.getY(), l.getZ());
            temp = (Point(u.getX(), u.getY(), u.getZ())).transform(rotation.getMatrix());
            u = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            break;
        case '6':
            rotation = Rotation(-speed, l.getX(), l.getY(), l.getZ());
            temp = (Point(u.getX(), u.getY(), u.getZ())).transform(rotation.getMatrix());
            u = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x, int y) {
    double speed = 5;

    calculateLR();

    switch (key) {
        case GLUT_KEY_LEFT:
            eye = eye - speed * r;
            center = center - speed * r;
            break;
        case GLUT_KEY_RIGHT:
            eye = eye + speed * r;
            center = center + speed * r;
            break;
        case GLUT_KEY_UP:
            eye = eye + speed * l;
            center = center + speed * l;
            break;
        case GLUT_KEY_DOWN:
            eye = eye - speed * l;
            center = center - speed * l;
            break;
        case GLUT_KEY_PAGE_UP:
            eye = eye + speed * u;
            center = center + speed * u;
            break;
        case GLUT_KEY_PAGE_DOWN:
            eye = eye - speed * u;
            center = center - speed * u;
            break;
        default:
            return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

void openglMain(int argc, char **argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(dimension, dimension);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
}

int main(int argc, char **argv) {
    // freopen("out.txt", "w", stdout);
    l = center - eye;
    l.normalize();
    u.normalize();
    r = l * u;
    u = r * l;
    loadData();
    loadTexture();
    openglMain(argc, argv);
    return 0;
}