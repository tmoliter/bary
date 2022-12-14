#include "UIRenderer.h"

int UIRenderer::currentPhraseId = 0;
UIRenderer *UIRenderer::u = new UIRenderer();

void UIRenderer::addPhrase(Phrase *p) {
    u->phrases.push_back(p);
    p->reset();
};

void UIRenderer::addText(Text *t) {
    u->texts.push_back(t);
};

void UIRenderer::addLine(int &parentX, int &parentY, Ray* ray, LineType type) {
    u->lines.push_back(new Line(parentX, parentY, ray, type));
}

void UIRenderer::addLines(int &parentX, int &parentY, vector<Ray*> rays, LineType type) {
    for (auto r : rays)
        u->lines.push_back(new Line(parentX, parentY, r, type));
}

void UIRenderer::renderPhrases() {
    vector<Phrase*>::iterator itr = u->phrases.begin();
    while (itr != u->phrases.end()) {
        Phrase* p = *itr;
        if (p->isComplete() && p->autoDestroy) {
            delete p;
            removePhrase(p);
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
    if (a->type == LineType::highlight)
        return false;
    if (b->type == LineType::highlight)
        return true;
    if (a->type == LineType::editing)
        return true;
    if (a->type == LineType::obstruction && b->type != LineType::editing)
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
    vector <Line*>::iterator itr = u->lines.begin();
    while (itr != u->lines.end()) {
        Line* l = *itr;
        if (l->ray == r) {
            delete l;
            itr = u->lines.erase(itr);
            continue;
        }
        itr++;
    }
}


void UIRenderer::removePhrase(Phrase *p) {
    u->phrases.erase(remove(u->phrases.begin(), u->phrases.end(), p), u->phrases.end());
}

void UIRenderer::removeText(Text *t) {
    delete t;
    u->texts.erase(remove(u->texts.begin(), u->texts.end(), t), u->texts.end());
}

void UIRenderer::changeLineType(Ray *r, LineType lineType) {
    for (auto l : u->lines) {
        if (l->ray == r) {
            l->type = lineType;
            return;
        }
    }
};
