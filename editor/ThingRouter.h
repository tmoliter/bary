#ifndef THING_ROUTER_H
#define THING_ROUTER_H
#include "editor/ThingEditor.h"

using namespace std;

enum class ThingRouterState {
    editOrCreateSub,
    chooseThingType,
    edit
};

struct ThingRouter {
    ThingRouter(Point p);
    ThingRouter(RealThing *rt);
    ~ThingRouter();


    Point newThingPosition;

    ThingRouterState state;

    ThingType type;
    string input;
    Text *text;
    RealThing *realThing;
    Sprite *cross;

    ThingEditor *thingEditor;

    void determineType();

    void createCross();
    void destroyCross();

    void changeState(ThingRouterState newState);

    int routeInput(KeyPresses keysDown);
    int chooseNewThingType(KeyPresses keysDown);
    int chooseEditAction(KeyPresses keysDown);
    int editThing(KeyPresses keysDown);
};

#endif