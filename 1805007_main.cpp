#include <bits/stdc++.h>

using namespace std;

#include "1805007_classes.h"

string scene_file = "scene_test.txt";

int level_of_recursion;
int dimension;

int number_of_objects;
int number_of_point_light_sources;
int number_of_spot_light_sources;

double floorWidth, tileWidth;

// declaration

vector<Object *> objects;
vector<PointLight *> pointLights;
vector<SpotLight *> spotLights;

void loadData() {
    ifstream ifs(scene_file);
    ifs >> level_of_recursion;
    ifs >> dimension;
    
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
        } else if (obj_type == "triangle") {
            Vector3D p1, p2, p3;
            Color color;
            CoEfficients coEfficients;
            int shine;
            ifs >> p1 >> p2 >> p3 >> color >> coEfficients >> shine;

            Object *triangle = new Triangle(p1, p2, p3);
            triangle->setColor(color);
            triangle->setCoEfficients(coEfficients);
            triangle->setShine(shine);

            objects.push_back(triangle);
        } else if (obj_type == "general") {

        }
    }

    // ifs >> number_of_point_light_sources;
    // for (int i = 0; i < number_of_point_light_sources; ++i) {
    //     Vector3D lightPos;
    //     Color color;
        
    //     ifs >> lightPos;
    //     ifs >> color;

    //     PointLight *pointLight = new PointLight(lightPos, color);
    //     pointLights.push_back(pointLight);
    // }

    // ifs >> number_of_spot_light_sources;
    // for (int i = 0; i < number_of_spot_light_sources; ++i) {
    //     PointLight pointLight;
    //     Vector3D lightDirection;
    //     double cutoffAngle;

    //     ifs >> pointLight >> lightDirection >> cutoffAngle;

    //     SpotLight *spotLight = new SpotLight(pointLight, lightDirection, cutoffAngle);
    //     spotLights.push_back(spotLight);
    // }

    // Floor

    floorWidth = 1000;
    tileWidth = 20;

    Object *floor = new Floor(floorWidth, tileWidth);
    // setColor
    // setCoEfficients
    // setShine
    objects.push_back(floor);

    ifs.close(); 
}

void capture() {
    // implement
}

// OpenGL Codes

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 100, eyey = 100, eyez = 100;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;

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
    gluLookAt(eyex,eyey,eyez,
              centerx,centery,centerz,
              upx,upy,upz);
    // draw
    drawAxes();

    for (Object *object : objects) {
        object->draw();
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
    gluPerspective(45.0f, aspect, 0.1f, 20000);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 10;
    Point center, eye, up, look, right;
    Rotation rotation;

    center = Point(centerx, centery, centerz);
    eye = Point(eyex, eyey, eyez);
    up = Point(upx, upy, upz);
    look = center - eye;
    right = look * up;

    switch (key) {
        case 'a':
            rotation = Rotation(-10, up.getPx(), up.getPy(), up.getPz());
            eye = eye - center;
            eye = eye.transform(rotation.getMatrix());
            eye = eye + center;
            eyex = eye.getPx();
            eyey = eye.getPy();
            eyez = eye.getPz();
            right = look * up;
            break;
        case 'd':
            rotation = Rotation(10, up.getPx(), up.getPy(), up.getPz());
            eye = eye - center;
            eye = eye.transform(rotation.getMatrix());
            eye = eye + center;
            eyex = eye.getPx();
            eyey = eye.getPy();
            eyez = eye.getPz();
            right = look * up;
            break;
        case '1':
            rotation = Rotation(10, up.getPx(), up.getPy(), up.getPz());
            center = center - eye;
            center = center.transform(rotation.getMatrix());
            center = center + eye;
            centerx = center.getPx();
            centery = center.getPy();
            centerz = center.getPz();
            right = look * up;
            break;
        case '2':
            rotation = Rotation(-10, up.getPx(), up.getPy(), up.getPz());
            center = center - eye;
            center = center.transform(rotation.getMatrix());
            center = center + eye;
            centerx = center.getPx();
            centery = center.getPy();
            centerz = center.getPz();
            right = look * up;
            break;
        case '3':
            rotation = Rotation(10, right.getPx(), right.getPy(), right.getPz());
            center = center - eye;
            center = center.transform(rotation.getMatrix());
            center = center + eye;
            centerx = center.getPx();
            centery = center.getPy();
            centerz = center.getPz();
            break;
        case '4':
            rotation = Rotation(-10, right.getPx(), right.getPy(), right.getPz());
            center = center - eye;
            center = center.transform(rotation.getMatrix());
            center = center + eye;
            centerx = center.getPx();
            centery = center.getPy();
            centerz = center.getPz();
            break;
        case '5':
            rotation = Rotation(10, look.getPx(), look.getPy(), look.getPz());
            up = up.transform(rotation.getMatrix());
            upx = up.getPx();
            upy = up.getPy();
            upz = up.getPz();
            right = look * up;
            break;
        case '6':
            rotation = Rotation(-10, look.getPx(), look.getPy(), look.getPz());
            up = up.transform(rotation.getMatrix());
            upx = up.getPx();
            upy = up.getPy();
            upz = up.getPz();
            right = look * up;
            break;
        // Control exit
        case 27:    // ESC key
            exit(0);    // Exit window
            break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x, int y) {
    double v = 10;
    double lx = centerx - eyex;
    double ly = centery - eyey;
    double lz = centerz - eyez;
    double s;
    double speed = 10;
    double m = 1.0;
    double kk = 0.1;

    double magnitude;

    double rightx = ly * upz - upy * lz, righty = upx * lz - lx * upz, rightz = lx * upy - upx * ly;
    double right_magnitude = sqrt(rightx * rightx + righty * righty + rightz * rightz);
    if (right_magnitude != 0) {
        rightx /= right_magnitude;
        righty /= right_magnitude;
        rightz /= right_magnitude;
    }

    switch (key) {
        case GLUT_KEY_LEFT:
            eyex -= speed * rightx;
            eyey -= speed * righty;
            eyez -= speed * rightz;
            centerx -= speed * rightx;
            centery -= speed * righty;
            centerz -= speed * rightz;
            break;
        case GLUT_KEY_RIGHT:
            eyex += speed * rightx;
            eyey += speed * righty;
            eyez += speed * rightz;
            centerx += speed * rightx;
            centery += speed * righty;
            centerz += speed * rightz;
            break;
        case GLUT_KEY_UP:
            magnitude = sqrt((centerx - eyex) * (centerx - eyex) + (centery - eyey) * (centery - eyey) + (centerz - eyez) * (centerz - eyez));
            eyex += (centerx - eyex) / magnitude * speed;
            eyey += (centery - eyey) / magnitude * speed;
            eyez += (centerz - eyez) / magnitude * speed;
            break;
        case GLUT_KEY_DOWN:
            magnitude = sqrt((centerx - eyex) * (centerx - eyex) + (centery - eyey) * (centery - eyey) + (centerz - eyez) * (centerz - eyez));
            eyex -= (centerx - eyex) / magnitude * speed;
            eyey -= (centery - eyey) / magnitude * speed;
            eyez -= (centerz - eyez) / magnitude * speed;
            break;
        case GLUT_KEY_PAGE_UP:
            eyex += speed * upx;
            eyey += speed * upy;
            eyez += speed * upz;
            centerx += speed * upx;
            centery += speed * upy;
            centerz += speed * upz;
            break;
        case GLUT_KEY_PAGE_DOWN:
            eyex -= speed * upx;
            eyey -= speed * upy;
            eyez -= speed * upz;
            centerx -= speed * upx;
            centery -= speed * upy;
            centerz -= speed * upz;
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