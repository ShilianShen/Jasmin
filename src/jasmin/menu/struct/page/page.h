#ifndef JASMIN_MENU_STRUCT_PAGE_H
#define JASMIN_MENU_STRUCT_PAGE_H


#include "../elem/elem.h"


typedef char PageName;
typedef int PageId;
typedef int PageVolume;


#define PAGE_MAX_VOLUME 16


typedef struct Page {
    PageId id;
    PageName* name;  // malloc
    Elem elems[PAGE_MAX_VOLUME];
} Page;


bool testPage(const Page* page, const char* string);


#endif //JASMIN_MENU_STRUCT_PAGE_H
