#ifndef JASMIN_MENU_STRUCT_PAGE_H
#define JASMIN_MENU_STRUCT_PAGE_H


#include "elem.h"


typedef struct Page Page;


Page* TEMPO_CreatePage(const cJSON* page_json);
Page *TEMPO_DeletePage(Page *page);
void TEMPO_PrintPage(const Page* page);
bool TEMPO_RenewPage(Page *page);
bool TEMPO_DrawPage(const Page *page);


#endif //JASMIN_MENU_STRUCT_PAGE_H