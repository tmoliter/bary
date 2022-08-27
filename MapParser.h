#ifndef MAP_PARSER_H
#define MAP_PARSER_H
#include <string>
#include <cstring>
#include <fstream>
#include <typeinfo>
#include "./things/Thing.h"
#include "./ThingList.h"
#include "./Camera.h"

using namespace std;

void parse_entity(ifstream &mapData, ThingData &newTD);

void parse_map(ThingList &thingList, Camera *c);

class MapParser {

};

#endif