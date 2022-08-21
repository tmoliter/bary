#ifndef INPUT_H
#define INPUT_H

struct KeyPresses {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool ok = false;
    bool cancel = false;
    bool menu1 = false;
    bool menu2 = false;
    bool start = false;
    bool quit = false;
};

class Input {
    private:
        KeyPresses keysDown;
    public:
        KeyPresses& getInput();
        Input() {KeyPresses keysDown;}
};

#endif
