#include "menu.h"


const char MENU_ROOT_NAME[] = "Root";
const char MENU_EDGE_NAME[] = "Edge";


Menu menu;


Page* MENU_getPageFromPathId(const int pathId) {
    return menu.pages[menu.path[pathId]];
}
Page* MENU_GetPageFromPageId(const int pageId) {
    return menu.pages[pageId];
}