#ifndef JASMIN_MENU_STRUCT_PAGE_H
#define JASMIN_MENU_STRUCT_PAGE_H


#include "../elem/elem.h"


typedef struct Page Page;


Page* TEMPO_CreatePage(const toml_table_t* tomlPage);
Page *TEMPO_DeletePage(Page *page);
bool TEMPO_RenewPage(const Page *page);
bool TEMPO_DrawPage(const Page *page);


#endif //JASMIN_MENU_STRUCT_PAGE_H