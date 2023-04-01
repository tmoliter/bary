#include "CommandLine.h"

CommandLine::CommandLine(vector<string> coms, bool oTM) : commands(coms), openTextMode(oTM), historyPosition(-1) {
    commandText = new Text(Point(48, 48), "");
    helpText = new Text(Point(32, 32), "");
    commandList = new Text(Point(SCREEN_WIDTH - 172, 16), "");
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
    c = new CommandLine({}, false);
}

void CommandLine::kill() {
    delete c;
    c = nullptr;
}


void CommandLine::refresh(vector<string> coms, bool oTM) {
    c->historyPosition = -1;
    c->commands = coms;
    c->openTextMode = oTM;
    c->input = "";
    c->commandText->setText("");
    gameState = GameState::TextInput;
    string cL;
    if (c->openTextMode) {
        c->helpText->setText("Open Text Entry: ");
        cL = "";
        // "Commands" in 'openTextMode' can be any additional information about what is being entered
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
    if (keysDown.textInput) {
        c->input.push_back(keysDown.textInput);
        c->commandText->setText(c->input);
        return 0;
    }
    if (keysDown.del && c->input.length() > 0){
        c->input.pop_back();
        c->commandText->setText(c->input);
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
        if (c->openTextMode || count(c->commands.begin(), c->commands.end(), c->input)) {
            c->history.push_front(c->input);
            if (c->history.size() > 15)
                c->history.pop_back();
            return 1;
        }
    }
    return 0;
}

string CommandLine::popInput() {
    string tmp = c->input;
    c->input.clear();
    return tmp;
}
