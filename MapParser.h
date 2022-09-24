#ifndef MAP_PARSER_H
#define MAP_PARSER_H
#include <string>
#include <cstring>
#include <fstream>
#include "./things/Thing.h"
#include "./things/FieldPlayer.h"
#include "./things/RealThing.h"
#include "./MapBuilder.h"
#include "./Camera.h"

using namespace std;

Thing *parse_thing(ifstream &mapData);
void parse_map(const char *mapPath);

#endif