#include <bits/stdc++.h>

using namespace std;

#include "1805007_classes.h"

string scene_file = "scene_test.txt";

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

            Object *pyramid = new Pyramid(lowest_point, width, height);
            pyramid->setColor(color);
            pyramid->setCoEfficients(coEfficients);
            pyramid->setShine(shine);

            objects.push_back(pyramid);
        } else if (obj_type == "cube") {
            Vector3D bottom_lower_left_point;
            double side;
            Color color;
            CoEfficients coEfficients;
            int shine;
            ifs >> bottom_lower_left_point >> side >> color >> coEfficients >> shine;

            Object *cube = new Cube(bottom_lower_left_point, side);
            cube->setColor(color);
            cube->setCoEfficients(coEfficients);
            cube->setShine(shine);

            objects.push_back(cube);
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

// // Global variables
// GLfloat eyex = 0, eyey = -100, eyez = 0;
// GLfloat centerx = 0, centery = 0, centerz = 0;
// GLfloat upx = 0, upy = 0, upz = 1;

int fileCount = 0;

Vector3D eye(20, -100, 20);
Vector3D center(20, 20, 20);
Vector3D u(0, 0, 1);
Vector3D l, r;

void calculateLR() {
    l = center - eye;
    l = l * (1 / sqrt(Vector3D::dot(l, l)));
    r = l * u;
    r = r * (1 / sqrt(Vector3D::dot(r, r)));
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

    for (int i = 1; i <= dimension; ++i) {
        for (int j = 1; j <= dimension; ++j) {
            Vector3D curPixel = topLeft + (i - 1) * du * r - (j - 1) * dv * u;
            Ray ray(curPixel, curPixel - eye);
            Color color(0, 0, 0), dummyColor;
            double tMin = 1e9;
            Object *nearestObject = nullptr;
            for (Object *object : objects) {
                double t = object->intersect(ray, dummyColor, level_of_recursion + 1);
                if (t >= 0 && t < tMin) {
                    tMin = t;
                    nearestObject = object;
                }
            }
            if (!nearestObject || tMin < 0 || tMin > 20000) continue;
            tMin = nearestObject->intersect(ray, color, level_of_recursion);
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
    double speed = 10;
    Rotation rotation;

    calculateLR();

    Point temp;

    switch (key) {
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
            break;
        case '4':
            rotation = Rotation(-speed, r.getX(), r.getY(), r.getZ());
            center = center - eye;
            temp = (Point(center.getX(), center.getY(), center.getZ())).transform(rotation.getMatrix());
            center = Vector3D(temp.getPx(), temp.getPy(), temp.getPz());
            center = center + eye;
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
    double speed = 10;

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
    loadData();
    openglMain(argc, argv);
    return 0;
}