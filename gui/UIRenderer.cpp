#include "UIRenderer.h"

int UIRenderer::currentPhraseId = 0;
UIRenderer *UIRenderer::u = new UIRenderer();

void UIRenderer::addPhrase(Phrase *p) {
    u->phrases.push_back(p);
};

void UIRenderer::addText(Text *t) {
    u->texts.push_back(t);
};

void UIRenderer::addLine(int &parentX, int &parentY, Ray* ray, LineType type) {
    u->lines.push_back(new Line(parentX, parentY, ray, type));
}

void UIRenderer::addLines(int &parentX, int &parentY, vector<Ray*> rays, LineType type) {
    for (auto r : rays) {
        u->lines.push_back(new Line(parentX, parentY, r, type));
    }
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

void UIRenderer::renderTexts() {
    for (auto t : u->texts) {
        t->render();
    }
}

bool _compareType (Line* a, Line* b) {
    if (a->type == obstruction)
        return true;
    return false;
}
void UIRenderer::renderLines() {
    sort(u->lines.begin(), u->lines.end(), _compareType);
    for (auto l : u->lines) {
        l->render();
    }
}

void UIRenderer::render() {
    renderLines();
    renderPhrases();
    renderTexts();
}

void UIRenderer::removeLine(Ray *r) {
vector <Line*>::iterator it;
for (it = u->lines.begin(); it != u->lines.end(); ++it) {
    Line* l = *it;
    if (l->ray == r) {
        delete l;
        it = u->lines.erase(it);
        break;
        }
    }
}

void UIRenderer::removePhrase(Phrase *p) {
    u->phrases.erase(remove(u->phrases.begin(), u->phrases.end(), p), u->phrases.end());
}

void UIRenderer::removeText(Text *t) {
    delete t;
    u->texts.erase(remove(u->texts.begin(), u->texts.end(), t), u->texts.end());
}
