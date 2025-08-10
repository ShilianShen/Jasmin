#include "page.h"


// GET & SET ===========================================================================================================
//

// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreatePage_RK(Page* page, const char* name, const toml_table_t* tomlPage) {
    memset(page, 0, sizeof(Page));

    if (name != NULL) {
        page->name = strdup(name);
        if (page->name == NULL) {
            printf("%s: failed malloc page.name.\n", __func__);
            return false;
        }
    } // name
    else {
        printf("%s: name == NULL.\n", __func__);
        return false;
    }

    const char* key;
    if (toml_array_in(tomlPage, key = "elemSet") != NULL) {
        const toml_array_t* tomlElemSet = toml_array_in(tomlPage, key);
        if (tomlElemSet == NULL) {
            printf("%s: tomlElems not exists, %s.\n", __func__, key);
            return false;
        } // Req Condition

        page->lenElemSet = toml_array_nelem(tomlElemSet);
        if (page->lenElemSet == 0) {
            printf("%s: failed malloc page.lenElemSet.\n", __func__);
            return false;
        } // Req Condition

        page->elemSet = malloc(page->lenElemSet * sizeof(Elem*));
        if (page->elemSet == NULL) {
            printf("%s: failed malloc page.elemSet.\n", __func__);
            return false;
        } // Req Condition

        for (int i = 0; i < page->lenElemSet; i++) {
            const toml_table_t* tomlElem = toml_table_at(tomlElemSet, i);
            if (tomlElem == NULL) {
                printf("%s: failed malloc page.elemSet.\n", __func__);
                return false;
            } // Req Condition

            page->elemSet[i] = TEMPO_CreateElem(tomlElem); // malloc
        }
    } // lenElemSet, elemSet
    return true;
}
static bool TEMPO_CreatePage_CK(const Page* page) {
    for (int i = 0; i < page->lenElemSet; i++) {
        if (page->elemSet[i] == NULL) {
            printf("%s: elemSet[%d] == NULL.\n", __func__, i);
            return false;
        }
    }
    return true;
}





Page* TEMPO_DeletePage(Page* page) {
    if (page != NULL) {
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
    if (TEMPO_CreatePage_RK(page, name, tomlPage) == false || TEMPO_CreatePage_CK(page) == false) {
        printf("%s: RK or CK == false.\n", __func__);
        page = TEMPO_DeletePage(page);
    } // Req Condition
    return page;
}


// RENEW ===============================================================================================================
bool TEMPO_RenewPage(const Page *page) {
    // Req Condition
    if (page == NULL) {
        DEBUG_SendMessageR("%s: page not exists.\n", __func__);
        return false;
    }

    //
    for (int i = 0; i < page->lenElemSet; i++) {
        if (page->elemSet[i] != NULL) {
            TEMPO_RenewElem(page->elemSet[i]);
        }
    }
    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawPage(const Page* page) {
    // Req Condition
    if (page == NULL) {
        DEBUG_SendMessageR("%s: page not exists.\n", __func__);
        return false;
    }

    //
    for (int i = 0; i < page->lenElemSet; i++) {
        if (page->elemSet[i] != NULL) {
            TEMPO_DrawElem(page->elemSet[i]);
        }
    }
    return true;
}