#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include <iostream>
#include <string>
#include <deque>
#include <algorithm>
#include "gui/UIRenderer.h"

using namespace std;

enum class CLIMode {
    openText,
    typeCommand,
    select,
};

struct CommandLine {
    CommandLine(vector<string> coms, CLIMode m);
    ~CommandLine();

    static void init();
    static void kill();
    static void refresh(vector<string> coms, CLIMode m);
    static void breakdown();

    CLIMode mode;
    vector<string> commands;
    int historyPosition, currentChoice;
    string input, lastPath;
    Text *commandText, *helpText, *commandList;


    static int handleInput(KeyPresses keysDown);
    static int handleTextInput(KeyPresses keysDown);
    static int handleSelectInput(KeyPresses keysDown);
    static void refreshCommandList();

    static string popInput();
    static string getChoice();

    deque<string> history;
    inline static CommandLine *c = nullptr;
};

#endif
