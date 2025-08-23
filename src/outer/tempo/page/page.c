#include "page.h"

// PAGE ================================================================================================================
struct Page {
    int lenElemSet;
    Elem** elemSet;
};


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreatePage_RK(Page* page, const cJSON* page_json) {
    memset(page, 0, sizeof(Page));

    const char* key;
    if (cJSON_GetObjectItem(page_json, key = "elemSet") != NULL) {
        const cJSON* elemSet_json = cJSON_GetObjectItem(page_json, key);
        if (elemSet_json == NULL) {
            printf("%s: tomlElems not exists, %s.\n", __func__, key);
            return false;
        } // Req Condition

        page->lenElemSet = cJSON_GetArraySize(elemSet_json);
        if (page->lenElemSet == 0) {
            printf("%s: failed malloc page.lenElemSet.\n", __func__);
            return false;
        } // Req Condition

        page->elemSet = calloc(page->lenElemSet, sizeof(Elem*));
        if (page->elemSet == NULL) {
            printf("%s: failed malloc page.elemSet.\n", __func__);
            return false;
        } // Req Condition

        for (int i = 0; i < page->lenElemSet; i++) {
            const cJSON* elem_json = cJSON_GetArrayItem(elemSet_json, i);
            if (elem_json == NULL) {
                printf("%s: failed malloc page.elemSet.\n", __func__);
                return false;
            } // Req Condition

            page->elemSet[i] = TEMPO_CreateElem(elem_json); // malloc
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
Page* TEMPO_CreatePage(const cJSON* page_json) {
    if (page_json == NULL) {
        printf("%s: tomlPage == NULL.\n", __func__);
        return NULL;
    } // Req Condition
    Page* page = calloc(1, sizeof(Page));
    if (page == NULL) {
        printf("%s: page == NULL.\n", __func__);
        return page;
    } // Req Condition
    if (TEMPO_CreatePage_RK(page, page_json) == false || TEMPO_CreatePage_CK(page) == false) {
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
        const bool renew = TEMPO_RenewElem(page->elemSet[i]);
        if (renew == false) {
            DEBUG_SendMessageR("%s: renew == false.\n", __func__);
            return false;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderClear(renderer);
    for (int i = 0; i < page->lenElemSet; i++) {
        const bool draw = TEMPO_DrawElem(page->elemSet[i]);
        if (draw == false) {
            DEBUG_SendMessageR("%s: draw[%d] == false.\n", __func__, i);
            return false;
        }
    }
    return true;
}