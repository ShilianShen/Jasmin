#ifndef JASMIN_MENU_FUNC_H
#define JASMIN_MENU_FUNC_H


#include "interface.h"


void MENU_InitElem(Elem* elem);
void MENU_InitPage(Page* page);
void MENU_InitMenu(SDL_Renderer* renderer);
void MENU_Init(SDL_Renderer* renderer);

void MENU_DeinitElem(const Elem* elem);
void MENU_DeinitPage(const Page* page);
void MENU_DeinitMenu();
void MENU_Deinit();


SDL_Texture* getTextureFromElemString(const char* elemStr);


void MENU_LoadElemGuide(Elem* elem, const toml_array_t* tomlGuide);
void MENU_LoadElemString(Elem* elem, const toml_datum_t tomlString);
void MENU_LoadElemPara(Elem* elem, const toml_datum_t tomlPara);
void MENU_LoadElemTexture(Elem* elem);
void MENU_LoadElemOther(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId);
void MENU_LoadElem(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId);


toml_table_t* getToml(const char* tomlPath);
void MENU_LoadPageName(Page* page, const char* name);
void MENU_LoadPageElems(Page* page, const toml_array_t* tomlElems);
void MENU_LoadPage(Page* page, const char* name, const toml_table_t* tomlPage);


void MENU_LoadMenuThemeFont(const toml_datum_t tomlFontPath, const toml_datum_t tomlFontSize);
void MENU_LoadMenuThemeColor(const toml_array_t* tomlColor);
void MENU_LoadMenuTheme(const char* tomlPath);
void MENU_LoadMenuPages(const char* tomlPath);
void MENU_LoadMenu(const char* menuPagesPath, const char* menuThemePath);
void MENU_Load(const char* menuPagesPath, const char* menuThemePath);

void MENU_Unload();


void MENU_Renew();
void MENU_Draw();


#endif //JASMIN_MENU_FUNC_H