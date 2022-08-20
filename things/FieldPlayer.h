#include "Thing.h"
#include "../components/Walk.h"

class FieldPlayer : public Thing {
    public:
        FieldPlayer(int x, int y, SDL_Renderer* renderer) : Thing(x,y,renderer) {};
        void init();
        void premeat() {};
        void meat();
        void render();
        void destroy();
    private:
        Walk* walk;
};
