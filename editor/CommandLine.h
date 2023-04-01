#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include <iostream>
#include <string>
#include <deque>
#include <algorithm>
#include "globals.h"
#include "gui/Text.h"
#include "gui/UIRenderer.h"

using namespace std;

struct CommandLine {
    CommandLine(vector<string> c, bool oTM);
    ~CommandLine();

    void init(vector<string> c, bool oTM);

    bool openTextMode;

    vector<string> commands;
    int historyPosition;
    string input, lastPath;
    Text *commandText, *helpText, *commandList;

    string popInput();

    int handleInput(KeyPresses keysDown);

    // THE FUTURE: history should be a static inline Array of 10 rather than storing it per
    // object and passing it around
    inline static deque<string> history;
};

#endif
