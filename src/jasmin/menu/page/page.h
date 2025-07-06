#ifndef JASMIN_MENU_STRUCT_PAGE_H
#define JASMIN_MENU_STRUCT_PAGE_H


#include "../elem/elem.h"


#define PAGE_ELEM_VOLUME 16


typedef struct Page {
    int id; // id < 0 means it's off
    char* name; // malloc in LOAD
    Elem elems[PAGE_ELEM_VOLUME];
} Page;


void PAGE_Init(Page* page);
void PAGE_Deinit(const Page* page);
void PAGE_Load(Page* page, const char* name, const toml_table_t* tomlPage);
void PAGE_Unload(Page* page);
void PAGE_Renew(Page* page);
void PAGE_Draw(const Page* page);


bool PAGE_IfReady(const Page* page);


#endif //JASMIN_MENU_STRUCT_PAGE_H
