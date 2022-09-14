#include "./SimpleMessageI.h"

SimpleMessageI::SimpleMessageI(Thing* parent, string message, CollidableData cd) : Interactable(parent, cd) {

}

SimpleMessageI::~SimpleMessageI() {
    interactables.erase(id);
}
