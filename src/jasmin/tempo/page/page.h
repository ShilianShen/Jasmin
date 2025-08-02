#ifndef JASMIN_MENU_STRUCT_PAGE_H
#define JASMIN_MENU_STRUCT_PAGE_H


#include "../elem/elem.h"


typedef struct PageInfo PageInfo;
typedef struct Page {
    bool ok;
    int id; // id < 0 means it's off
    char* name; // malloc in LOAD
    int numElems;
    Elem** elems;
} Page;


Page* TEMPO_CreatePage(const char* name, const toml_table_t* tomlPage);
void TEMPO_DestroyPage(Page* page);
void TEMPO_RenewPage(Page* page);
void TEMPO_DrawPage(const Page* page);


bool TEMPO_GetPageOk(const Page* page);


#endif //JASMIN_MENU_STRUCT_PAGE_H
