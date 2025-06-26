#ifndef JASMIN_MENU_STRUCT_PAGE_H
#define JASMIN_MENU_STRUCT_PAGE_H


#include "../elem/elem.h"


typedef char PageName;
typedef int PageId;
typedef int PageVolume;
const PageVolume PAGE_MAX_VOLUME = 32;


typedef struct {
    PageId id;
    PageName* name;  // malloc
    Elem elems[PAGE_MAX_VOLUME];
} Page;


bool testPage(const Page* page, const char* string) {
    if (page == NULL) {if (string != NULL) {printf("%s: page not exists.\n", string);} return false;}
    if (page->name == NULL) {if (string != NULL) {printf("%s: name not exists.\n", string);} return false;}
    return true;
}


#endif //JASMIN_MENU_STRUCT_PAGE_H
