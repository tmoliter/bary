#ifndef THING_ROUTER_H
#define THING_ROUTER_H
#include "editor/ThingEditor.h"
#include "editor/DoorEditor.h"

using namespace std;

enum class ThingType {
    thing,
    door
};

enum class ThingRouterState {
    editOrCreateSub,
    chooseThingType,
    edit
};

struct ThingRouter {
    ThingRouter(Point p);
    ThingRouter(Door *door);
    ThingRouter(RealThing *rt);
    ~ThingRouter();


    Point newThingPosition;

    ThingRouterState state;

    ThingType type;
    string input;
    Text *text;
    RealThing *realThing;

    ThingEditor *thingEditor;
    DoorEditor *doorEditor;

    void determineType();

    void changeState(ThingRouterState newState);

    int routeInput(KeyPresses keysDown);
    int chooseNewThingType(KeyPresses keysDown);
    int chooseEditAction(KeyPresses keysDown);
    int editThing(KeyPresses keysDown);
};

#endif