#include <bits/stdc++.h>
using namespace std;

#include "1805007_classes.h"

string scene_file = "scene.txt";

int level_of_recursion;
int dimension;

int number_of_objects;

void loadData() {
    ifstream ifs(scene_file);
    ifs >> level_of_recursion;
    ifs >> dimension;
    ifs >> number_of_objects;
    for (int i = 0; i < number_of_objects; ++i) {
        string obj_type;
        ifs >> obj_type;
        if (obj_type == "sphere") {
            
        } else if (obj_type == "triangle") {

        } else if (obj_type == "general") {

        }
    }
    ifs.close();    
}

int main(int argc, char *argv[]) {
    // loadData();
    return 0;   
}