#include <bits/stdc++.h>
using namespace std;

#include "1805007_classes.h"

string scene_file = "scene.txt";

int level_of_recursion;
int dimension;

int number_of_objects;
int number_of_point_light_sources;
int number_of_spot_light_sources;

double floorWidth;
double tileWidth;

// declaration

extern vector<Object *> objects;
extern vector<PointLight *> pointLights;
extern vector<SpotLight *> spotLights;

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

        } else if (obj_type == "general") {

        }
    }

    ifs >> number_of_point_light_sources;
    for (int i = 0; i < number_of_point_light_sources; ++i) {
        Vector3D lightPos;
        Color color;
        
        ifs >> lightPos;
        ifs >> color;

        PointLight *pointLight = new PointLight(lightPos, color);
        pointLights.push_back(pointLight);
    }

    ifs >> number_of_spot_light_sources;
    for (int i = 0; i < number_of_spot_light_sources; ++i) {
        PointLight pointLight;
        Vector3D lightDirection;
        double cutoffAngle;

        ifs >> pointLight >> lightDirection >> cutoffAngle;

        SpotLight *spotLight = new SpotLight(pointLight, lightDirection, cutoffAngle);
        spotLights.push_back(spotLight);
    }

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

int main(int argc, char *argv[]) {
    // loadData();
    return 0;   
}