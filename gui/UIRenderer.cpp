#include "UIRenderer.h"

int UIRenderer::currentPhraseId = 0;
UIRenderer *UIRenderer::u = new UIRenderer();

void UIRenderer::addPhrase(Phrase *p) {
    u->phrases.push_back(p);
};

void UIRenderer::setText(string s) {
    text = s;
}

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


void UIRenderer::renderCenterText() {
    for (int i = 0; i < text.length();i++) {
        if (!Phrase::font) {
            SDL_Surface* temp = IMG_Load("assets/fonts/paryfont4rows.png");
            Phrase::font = SDL_CreateTextureFromSurface(renderer, temp);
            SDL_FreeSurface(temp);
        }
        int adjustedFontValue = int(text[i]) - 32;
        int fontX = (adjustedFontValue % LETTERS_PER_FONT_ROW) * LETTER_WIDTH;
        int fontY = (adjustedFontValue / LETTERS_PER_FONT_ROW) * LETTER_HEIGHT;
        SDL_Rect sourceRect = { fontX, fontY, LETTER_WIDTH, LETTER_HEIGHT};

        int xPosition = (SCREEN_WIDTH / 2) + (i * LETTER_WIDTH);
        int yPosition = (SCREEN_HEIGHT / 2);
        SDL_Rect renderRect = { xPosition, yPosition, LETTER_WIDTH, LETTER_HEIGHT };

        SDL_RenderCopy(renderer, Phrase::font, &sourceRect, &renderRect);
    }
}


void UIRenderer::render() {
    renderPhrases();
    renderCenterText();
}

void UIRenderer::removePhrase(Phrase *p) {
    u->phrases.erase(remove(u->phrases.begin(), u->phrases.end(), p), u->phrases.end());
}

void UIRenderer::clearText() {
    text.clear();
};
