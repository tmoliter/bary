#include "UIManager.h"

int UIManager::currentPhraseId = 0;
UIManager *UIManager::u = new UIManager();

void UIManager::addPhrase(Phrase *p) {
    u->phrases.push_back(p);
};

void UIManager::renderPhrases() {
    for (auto p : u->phrases)
        p->progDisplay(1);
}

void UIManager::removePhrase(Phrase *p) {
    u->phrases.erase(remove(u->phrases.begin(), u->phrases.end(), p), u->phrases.end());
}
