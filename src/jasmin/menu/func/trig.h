#ifndef FUNC_H
#define FUNC_H


void trigFuncPass(const TrigPara para) {}
void trigFuncForward(const TrigPara pageName) {
    ma_engine_play_sound(&maEngine, "../sound effects/ring01.wav", NULL);

    // getPageId
    PageId pageId = 0;
    for (PageId i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (!testPage(menu.pages[i], NULL)) {continue;}
        if (strcmp(menu.pages[i]->name, pageName) == 0) {pageId = i;}
    }
    if (pageId == 0) {printf("%s: \"%s\" not exists.\n", __func__, (char*)pageName); return;}

    // forward
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] == 0) {
            menu.path[i] = pageId;
            break;
        }
    }
}
void trigFuncBackward(const TrigPara para) {
    ma_engine_play_sound(&maEngine, "../sound effects/ring08.wav", NULL);

    for (PathId i = MENU_PATH_VOLUME - 1; i >= 0; i--) {
        if (menu.path[i] != 0) {
            menu.path[i] = 0;
            break;
        }
    }
}
void trigFuncClear(const TrigPara para) {
    ma_engine_play_sound(&maEngine, "../sound effects/ring08.wav", NULL);
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {menu.path[i] = 0;}
}


#endif //FUNC_H
