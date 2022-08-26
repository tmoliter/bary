#ifndef MAP_PARSER_H
#define MAP_PARSER_H
#include <string>
#include <cstring>
#include <fstream>
#include <typeinfo>
#include "./things/Thing.h"
#include "./ThingList.h"

using namespace std;

void parse_entity(ifstream &mapData, Thing *newThing);

void parse_map(ThingList &thingList);

class MapParser {

};

#endif