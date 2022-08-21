#include "Thing.h"
#include "../components/Walk.h"

class FieldPlayer : public Thing {
    public:
        FieldPlayer(int x, int y, SDL_Renderer* renderer) : Thing(x,y,renderer) {};
        void init();

        void premeat() {};
        void meat() {};
        void meat(KeyPresses keysDown);
        void render();

        void destroy();

        void getPosition(int &outerX, int &outerY);
    private:
        Walk* walk;
};
