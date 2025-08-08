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



// CREATE & DELETE =====================================================================================================
static void TEMPO_CreatePageName(Page* page, const char* name) {
    //
    page->name = strdup(name); // malloc page.name

    // Req Condition
    if (page->name == NULL) {
        printf("%s: failed malloc page.name.\n", __func__);
    }
}
static bool TEMPO_CreatePage_CK(const Page* page) {
    return page != NULL;
}
static bool TEMPO_CreatePage_RK(Page* page, const char* name, const toml_table_t* tomlPage) {
    if (name == NULL) {
        printf("%s: name not exists.\n", __func__);
        return false;
    } // Req Condition
    memset(page, 0, sizeof(Page));
    //
    TEMPO_CreatePageName(page, name);

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
            page->elemSet[i] = TEMPO_CreateElem(tomlElem); // malloc
        }
    }
    return true;
}


Page* TEMPO_DeletePage(Page* page) {
    if (page != NULL) {
        if (page == NULL) {
            printf("%s: page not exists.\n", __func__);
            return page;
        } // Req Condition
        if (page->name != NULL) {
            free(page->name);
            page->name = NULL;
        }
        if (page->elemSet != NULL) {
            for (int i = 0; i < page->lenElemSet; i++) {
                if (page->elemSet[i] != NULL) {
                    page->elemSet[i] = TEMPO_DeleteElem(page->elemSet[i]); // free
                }
            }
            free(page->elemSet);
            page->elemSet = NULL;
        }

        free(page);
        page = NULL;
    }
    return page;
}
Page* TEMPO_CreatePage(const char* name, const toml_table_t* tomlPage) {
    if (tomlPage == NULL) {
        printf("%s: tomlPage == NULL.\n", __func__);
        return NULL;
    } // Req Condition
    Page* page = calloc(1, sizeof(Page));
    if (page == NULL) {
        printf("%s: page == NULL.\n", __func__);
        return page;
    } // Req Condition

    TEMPO_CreatePage_RK(page, name, tomlPage);
    if (TEMPO_CreatePage_CK(page) == false) {
        printf("%s: failed malloc.\n", __func__);
        page = TEMPO_DeletePage(page);
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