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
    CommandLine(vector<string> coms, bool oTM);
    ~CommandLine();


    static void init();
    static void kill();
    static void refresh(vector<string> c, bool oTM);
    static void breakdown();

    bool openTextMode;
    vector<string> commands;
    int historyPosition;
    string input, lastPath;
    Text *commandText, *helpText, *commandList;

    static string popInput();

    static int handleInput(KeyPresses keysDown);

    deque<string> history;
    inline static CommandLine *c = nullptr;
};

#endif
