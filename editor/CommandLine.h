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
    CommandLine(vector<string> c, bool oTM);
    ~CommandLine();

    bool openTextMode;

    vector<string> commands;
    vector<string> history;
    string input, lastPath;
    Text *commandText, *helpText, *commandList;

    string popInput();

    void setHistory(vector<string> historyCommands);
    vector<string> getHistory();
    void clearHistory();
 
    int handleInput(KeyPresses keysDown);
};

#endif
