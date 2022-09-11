#include "UIManager.h"

int UIManager::currentPhraseId = 0;
UIManager *UIManager::u = new UIManager();

void UIManager::meat (KeyPresses keysDown) {
    if(keysDown.debug_1 && u->phrases.count(u->activePhrase)) {
        u->phrases[u->activePhrase]->advance();
    }
}

void UIManager::addPhrase(Point o, int pixelWidth, int pixelHeight, int scale, ScrollType type, string t, bool setAsActive) {
    Phrase *phrase = new Phrase(o, pixelWidth, pixelHeight, scale, type, t);
    u->phrases[currentPhraseId] = phrase;
    if(setAsActive)
        u->activePhrase = currentPhraseId;
    currentPhraseId++;
};

void UIManager::renderPhrases() {
    for (auto const& [id, p] : u->phrases){
        if(!p->progDisplay(1)) {
            u->phrasesToDelete.push_back(id);
        }
    }
}

void UIManager::cleanUp() {
    for (auto id : u->phrasesToDelete){
        Phrase *p = u->phrases[id];
        delete p;
        u->phrases.erase(id);
        if(id == u->activePhrase)
            u->activePhrase = -1;
    }
    u->phrasesToDelete.clear();
}

