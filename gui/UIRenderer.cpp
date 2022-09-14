#include "UIRenderer.h"

int UIRenderer::currentPhraseId = 0;
UIRenderer *UIRenderer::u = new UIRenderer();

void UIRenderer::addPhrase(Phrase *p) {
    u->phrases.push_back(p);
};

void UIRenderer::renderPhrases() {
   vector<Phrase*>::iterator itr = u->phrases.begin();
   while (itr != u->phrases.end()) {
        Phrase* p = *itr;
        if (p->isComplete() && p->autoDestroy) {
            delete p;
            u->phrases.erase(remove(u->phrases.begin(), u->phrases.end(), p), u->phrases.end());
            continue;
        }
        p->progDisplay();
        itr++;
   }
}

void UIRenderer::removePhrase(Phrase *p) {
    u->phrases.erase(remove(u->phrases.begin(), u->phrases.end(), p), u->phrases.end());
}
