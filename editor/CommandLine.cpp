#include "CommandLine.h"

CommandLine::CommandLine(vector<string> c) : commands(c) {
    gameState = GameState::TextInput;
    commandText = new Text(Point(32, 64), "");
    helpText = new Text(Point(32, 32), "Enter Command: ");
    string cL = "Commands:";
    for (auto s : commands)
        cL = cL + "` " + s;
    commandList = new Text(Point(32, 32), cL);
    UIRenderer::addText(commandText);
}

CommandLine::~CommandLine() {
    gameState = GameState::FieldFree;
    input.clear();
    if (commandText == nullptr)
        return;
    UIRenderer::removeText(commandText);
    UIRenderer::removeText(helpText);
    UIRenderer::removeText(commandList);
    commandText = nullptr;
}

string CommandLine::handleInput(KeyPresses keysDown) {
    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        commandText->setText(input);
        return NULL;
    }
    if (keysDown.del && input.length() > 0){
        input.pop_back();
        commandText->setText(input);
        return NULL;
    }
    if (keysDown.start) {
        string submitted = input;
        input.clear();
        commandText->setText(input);
        if (count(commands.begin(), commands.end(), submitted)) {
            return submitted;
        }
        return NULL;
    }
    return NULL;
}