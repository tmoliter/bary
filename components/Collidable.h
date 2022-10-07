#ifndef COLLIDABLE_H
#define COLLIDABLE_H
#include <vector>
#include <string>
#include <fstream>
#include "Ray.h"
#include "gui/UIRenderer.h"
#include "gui/Line.h"

using namespace std;


enum class CollidableType {
    obstruction,
    interactable,
    trigger
};

struct CollidableData {
    int layer = 0;
    vector<Ray*> rays;
};

struct Collidable {
    Collidable (Point &pP, string &tN, CollidableData cd);
    Collidable (Point &pP, string &tN);
    ~Collidable();
    Point &parentPos;
    string &thingName;
    LineType lineType;
    bool active, linesVisible;
    int layer;

    vector<Ray*> rays;

    void addRay(Ray *r);
    void removeRay(int index);

    void showLines();
    void hideLines();

    bool isColliding(Ray &incoming, int incomingLayer);

    static int parse_collidable_datum(ifstream &mapData, CollidableData &newCD);
};

#endif
