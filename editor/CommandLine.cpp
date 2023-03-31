#include "CommandLine.h"

CommandLine::CommandLine(vector<string> c, bool oTM) : commands(c), openTextMode(oTM), historyPosition(-1) {
    gameState = GameState::TextInput;
    commandText = new Text(Point(32, 64), "");
    if (openTextMode) {
        helpText = new Text(Point(32, 32), "Open Text Entry: ");
        string cL = "";
        // Commands here can be any additional information about what is being entered
        for (auto s : commands)
            cL = cL + s + "`";
        commandList = new Text(Point(32, 32), cL);
        UIRenderer::addText(commandText);
        return;
    }
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


int CommandLine::handleInput(KeyPresses keysDown) {
    if (keysDown.textInput) {
        input.push_back(keysDown.textInput);
        commandText->setText(input);
        return 0;
    }
    if (keysDown.del && input.length() > 0){
        input.pop_back();
        commandText->setText(input);
        return 0;
    }
    if (keysDown.up && history.size() - 1 > historyPosition) {
        input = history[++historyPosition];
    }
    if (keysDown.down && historyPosition > -1) {
        if (--historyPosition > -1)
            input = history[historyPosition];
        else
            input = "";
    }
    if (keysDown.start) {
        string submitted = input;
        input.clear();
        commandText->setText(input);
        if (openTextMode || count(commands.begin(), commands.end(), submitted)) {
            history.push_front(submitted);
            if (history.size() > 15)
                history.pop_back();
            return 1;
        }
    }
    return 0;
}

string CommandLine::popInput() {
    string tmp = input;
    input.clear();
    return tmp;
}
