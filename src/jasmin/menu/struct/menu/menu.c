#include "menu.h"


//const PageId MENU_PAGE_VOLUME = 16;
//const PathId MENU_PATH_VOLUME = 6;
const char MENU_ROOT_NAME[] = "Root";
const char MENU_EDGE_NAME[] = "Edge";


Menu menu;


Page* getMenuPageFromPathId(const PathId pathId) {
    return menu.pages[menu.path[pathId]];
}
Page* getMenuPageFromPageId(const PageId pageId) {
    return menu.pages[pageId];
}