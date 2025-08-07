#include "page.h"


// GET & SET ===========================================================================================================
bool TEMPO_GetPageOk(const Page* page) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return false;
    }
    if (page->id < 0) {
        printf("%s: page.id is illegal.\n", __func__);
        return false;
    }
    if (page->name == NULL) {
        printf("%s: page[%d].name not exists.\n", __func__, page->id);
        return false;
    }

    //
    return true;
}


// CHECK ===============================================================================================================
static bool TEMPO_CheckPage(const Page* page) {
    return page != NULL;
}


// CREATE & DESTROY=====================================================================================================
static void TEMPO_LoadPageName(Page* page, const char* name) {
    //
    page->name = strdup(name); // malloc page.name

    // Req Condition
    if (page->name == NULL) {
        printf("%s: failed malloc page.name.\n", __func__);
    }
}
static bool TEMPO_LoadPage(Page *page, const char *name, const toml_table_t *tomlPage) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return false;
    }
    if (name == NULL) {
        printf("%s: name not exists.\n", __func__);
        return false;
    }
    if (tomlPage == NULL) {
        printf("%s: tomlPage not exists.\n", __func__);
        return false;
    }

    //
    *page = (Page){0};

    //
    TEMPO_LoadPageName(page, name);

    //
    const toml_array_t* tomlElems = toml_array_in(tomlPage, "elems");
    if (tomlElems == NULL) {
        printf("%s: tomlElems not exists.\n", __func__);
        return false;
    }
    page->lenElemSet = toml_array_nelem(tomlElems);

    //
    page->elemSet = malloc(page->lenElemSet * sizeof(Elem*));
    for (int i = 0; i < page->lenElemSet; i++) {
        const toml_table_t* tomlElem = toml_table_at(tomlElems, i);
        if (tomlElem != NULL) {
            page->elemSet[i] = TEMPO_CreateElemFromToml(tomlElem); // malloc
        }
    }
    return true;
}
static void TEMPO_UnloadPage(Page* page) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return;
    }
    if (page->name != NULL) {
        free(page->name);
        page->name = NULL;
    }
    if (page->elemSet != NULL) {
        for (int i = 0; i < page->lenElemSet; i++) {
            if (page->elemSet[i] != NULL) {
                TEMPO_DeleteElem(page->elemSet[i]); // free
                page->elemSet[i] = NULL;
            }
        }
        free(page->elemSet);
        page->elemSet = NULL;
    }
}
static Page* TEMPO_CreatePage_(const char* name, const toml_table_t* tomlPage) {
    Page* page = malloc(sizeof(Page));
    if (page == NULL) {
        printf("%s: failed malloc.\n", __func__);
        return NULL;
    }
    TEMPO_LoadPage(page, name, tomlPage);
    return page;
}

void TEMPO_DestroyPage(Page* page) {
    if (page != NULL) {
        TEMPO_UnloadPage(page);
        free(page);
        page = NULL;
    }
}
Page* TEMPO_CreatePage(const char* name, const toml_table_t* tomlPage) {
    Page* page = TEMPO_CreatePage_(name, tomlPage);
    if (TEMPO_CheckPage(page) == false) {
        printf("%s: failed malloc.\n", __func__);
        TEMPO_DestroyPage(page);
        page = NULL;
    }
    return page;
}


// RENEW ===============================================================================================================
void TEMPO_RenewPage(const Page* page) {
    // Req Condition
    if (page == NULL) {
        DEBUG_SendMessageR("%s: page not exists.\n", __func__);
        return;
    }

    //
    for (int i = 0; i < page->lenElemSet; i++) {
        if (page->elemSet[i] != NULL) {
            TEMPO_RenewElem(page->elemSet[i]);
        }
    }
}


// DRAW ================================================================================================================
void TEMPO_DrawPage(const Page* page) {
    // Req Condition
    if (page == NULL) {
        DEBUG_SendMessageR("%s: page not exists.\n", __func__);
        return;
    }

    //
    for (int i = 0; i < page->lenElemSet; i++) {
        if (page->elemSet[i] != NULL) {
            TEMPO_DrawElem(page->elemSet[i]);
        }
    }
}