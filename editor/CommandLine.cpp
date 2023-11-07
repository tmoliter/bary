#include "CommandLine.h"

CommandLine::CommandLine(vector<string> coms, CLIMode m) : commands(coms), mode(m), historyPosition(-1), currentChoice(0) {
    commandText = new Text(Point(48, 48), "");
    helpText = new Text(Point(32, 32), "");
    commandList = new Text(Point(settings.SCREEN_WIDTH - 172, 16), "");
    UIRenderer::addText(commandList);
    UIRenderer::addText(commandText);
    UIRenderer::addText(helpText);
}

CommandLine::~CommandLine() {
    UIRenderer::removeText(commandText);
    UIRenderer::removeText(helpText);
    UIRenderer::removeText(commandList);
}

void CommandLine::init() {
    c = new CommandLine({}, CLIMode::openText);
}

void CommandLine::kill() {
    delete c;
    c = nullptr;
}


void CommandLine::refresh(vector<string> coms, CLIMode m) {
    c->historyPosition = -1;
    c->commands = coms;
    c->mode = m;
    c->input = "";
    c->commandText->setText("");
    gameState = GameState::TextInput;
    string cL;
    if (c->mode ==CLIMode::openText) {
        c->helpText->setText("Open Text Entry: ");
        cL = "";
        // "Commands" in 'openText' can be any additional information about what is being entered
    } else {
        c->helpText->setText("Enter Command: ");
        cL = "Commands:";
    }
    for (auto s : c->commands)
        cL = cL + "` " + s;
    c->commandList->setText(cL);
}

void CommandLine::breakdown() {
    gameState = GameState::FieldFree;
    c->input.clear();
    c->commandText->setText("");
    c->helpText->setText("");
    c->commandList->setText("");
}

int CommandLine::handleInput(KeyPresses keysDown) {
    switch (c->mode) {
        case CLIMode::openText:
        case CLIMode::typeCommand:
            return handleTextInput(keysDown);
        case CLIMode::select:
            return handleSelectInput(keysDown);
    }
    return 0;
}

int CommandLine::handleTextInput(KeyPresses keysDown) {
    if (keysDown.textInput) {
        c->input.push_back(keysDown.textInput);
        c->commandText->setText(c->input);
        refreshCommandList();
        return 0;
    }
    if (keysDown.del && c->input.length() > 0){
        c->input.pop_back();
        c->commandText->setText(c->input);
        refreshCommandList();
        return 0;
    }
    if (keysDown.debug_up && ((int)c->history.size() - 1 > c->historyPosition)) {
        c->input = c->history[++c->historyPosition];
        c->commandText->setText(c->input);
        return 0;
    }
    if (keysDown.debug_down && c->historyPosition > -1) {
        if (--c->historyPosition > -1) {
            c->input = c->history[c->historyPosition];
            c->commandText->setText(c->input);
        } else {
            c->input = "";
            c->commandText->setText(c->input);
        }
        return 0;
    }
    if (keysDown.start) {
        if (c->mode == CLIMode::typeCommand) {
            vector<string> possibleCommands;
            for (auto const& command : c->commands)
                if (c->input.size() < 1 || command.rfind(c->input, 0) == 0)
                    possibleCommands.push_back(command);
            if (possibleCommands.size() != 1)
                return 0;
            c->input = possibleCommands.back();
        }
        c->history.push_front(c->input);
        if (c->history.size() > 15)
            c->history.pop_back();
        return 1;
    }
    return 0;
}

int CommandLine::handleSelectInput(KeyPresses keysDown) {
    return 0;
}

void CommandLine::refreshCommandList() {
    if (c->mode != CLIMode::typeCommand)
        return;
    string cL = "Commands:";
    for (auto const& command : c->commands)
        if (c->input.size() < 1 || command.rfind(c->input, 0) == 0)
            cL = cL + "` " + command;
    c->commandList->setText(cL);
}

string CommandLine::popInput() {
    string tmp = c->input;
    c->input.clear();
    return tmp;
}
