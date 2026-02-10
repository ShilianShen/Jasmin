#ifndef JASMIN_MENU_STRUCT_PAGE_H
#define JASMIN_MENU_STRUCT_PAGE_H


#include "elem.h"


typedef struct TEMPO_Page TEMPO_Page;


TEMPO_Page *TEMPO_CreatePage(const cJSON *page_json);
TEMPO_Page *TEMPO_DeletePage(TEMPO_Page *page);
bool TEMPO_RenewPage(TEMPO_Page *page);
bool TEMPO_DrawPage(const TEMPO_Page *page);


#endif //JASMIN_MENU_STRUCT_PAGE_H