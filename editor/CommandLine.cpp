#include "CommandLine.h"

CommandLine::CommandLine(vector<string> c, bool oTM) : commands(c), openTextMode(oTM), historyPosition(-1) {
    init(c, oTM);
    UIRenderer::addText(commandList);
    UIRenderer::addText(commandText);
    UIRenderer::addText(helpText);
}

CommandLine::~CommandLine() {
    gameState = GameState::FieldFree;
    input.clear();
    UIRenderer::removeText(commandText);
    UIRenderer::removeText(helpText);
    UIRenderer::removeText(commandList);
}

void CommandLine::init(vector<string> c, bool oTM) {
    commands = c;
    openTextMode = oTM;
    input = "";
    gameState = GameState::TextInput;
    commandText = new Text(Point(48, 48), "");
    string cL;
    if (openTextMode) {
        helpText = new Text(Point(32, 32), "Open Text Entry: ");
        cL = "";
        // "Commands" in 'openTextMode' can be any additional information about what is being entered
    } else {
        helpText = new Text(Point(32, 32), "Enter Command: ");
        cL = "Commands:";
    }
    for (auto s : commands)
        cL = cL + "` " + s;
    commandList = new Text(Point(SCREEN_WIDTH - 172, 16), cL);
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
    if (keysDown.debug_up && ((int)history.size() - 1 > historyPosition)) {
        input = history[++historyPosition];
        commandText->setText(input);
        return 0;
    }
    if (keysDown.debug_down && historyPosition > -1) {
        if (--historyPosition > -1) {
            input = history[historyPosition];
            commandText->setText(input);
        } else {
            input = "";
            commandText->setText(input);
        }
        return 0;
    }
    if (keysDown.start) {
        if (openTextMode || count(commands.begin(), commands.end(), input)) {
            history.push_front(input);
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
