#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include <iostream>
#include <string>
#include <algorithm>
#include "globals.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;

struct CommandLine {
    CommandLine(vector<string> c);
    ~CommandLine();

    vector<string> commands;
    string input, lastPath;
    Text *commandText, *helpText, *commandList;
 
    string handleInput(KeyPresses keysDown);
};

#endif
