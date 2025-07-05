#ifndef JASMIN_MENU_FUNC_H
#define JASMIN_MENU_FUNC_H


#include "interface.h"


// init
void ELEM_Init(Elem* elem);
void PAGE_Init(Page* page);
void MENU_InitMenu(SDL_Renderer* renderer);
void MENU_Init(SDL_Renderer* renderer);


// deinit
void ELEM_Deinit(const Elem* elem);
void PAGE_Deinit(const Page* page);
void MENU_DeinitMenu();
void MENU_Deinit();


// load
SDL_Texture* getTextureFromElemString(const char* elemStr);


void ELEM_LoadGuide(Elem* elem, const toml_array_t* tomlGuide);
void ELEM_LoadString(Elem* elem, const toml_datum_t tomlString);
void ELEM_LoadPara(Elem* elem, const toml_datum_t tomlPara);
void ELEM_LoadTexture(Elem* elem);
void ELEM_LoadOther(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId);
void ELEM_Load(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId);


toml_table_t* getToml(const char* tomlPath);
void PAGE_LoadName(Page* page, const char* name);
void PAGE_LoadElems(Page* page, const toml_array_t* tomlElems);
void PAGE_Load(Page* page, const char* name, const toml_table_t* tomlPage);


void MENU_LoadMenuThemeFont(const toml_datum_t tomlFontPath, const toml_datum_t tomlFontSize);
void MENU_LoadMenuThemeColor(const toml_array_t* tomlColor);
void MENU_LoadMenuTheme(const char* tomlPath);
void MENU_LoadMenuPages(const char* tomlPath);
void MENU_LoadMenu(const char* menuPagesPath, const char* menuThemePath);
void MENU_Load(const char* menuPagesPath, const char* menuThemePath);

// unload
void ELEM_Unload(Elem* elem);
void PAGE_Unload(Page* page);
void MENU_UnloadMenuTheme();
void MENU_UnloadMenu();
void MENU_Unload();

// renew
void ELEM_RenewDstRect(Elem* elem);
void ELEM_RenewState(Elem* elem);
void MENU_RenewElem(Elem* elem);


void MENU_RenewPage(Page* page);

void MENU_RenewMenuPath();
void MENU_RenewMenuPageNow();
void MENU_RenewMenu();
void MENU_Renew();

void MENU_DrawElem(const Elem* elem);
void MENU_DrawPage(const Page* page);
void MENU_DrawMenu();
void MENU_Draw();


#endif //JASMIN_MENU_FUNC_H