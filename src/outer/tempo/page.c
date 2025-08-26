#include "page.h"

// PAGE PARA ===========================================================================================================
const SDL_FRect* publicBck = NULL;


// PAGE ================================================================================================================
struct Page {
    int lenElemTable;
    KeyVal* elemTable;

    int anchor;
    SDL_FRect src_rect, *src;
    SDL_FRect dst_rect, *bck;
    SDL_Color color;
};


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreatePage_RK(Page* page, const cJSON* page_json) {
    memset(page, 0, sizeof(Page));
    const char* key;
    if (cJSON_ExistKey(page_json, key = "elemTable")) {
        const cJSON* elemTable_json = cJSON_GetObjectItem(page_json, key);
        if (elemTable_json == NULL) {
            printf("%s: tomlElems not exists, %s.\n", __func__, key);
            return false;
        } // Req Condition

        page->lenElemTable = cJSON_GetArraySize(elemTable_json);
        if (page->lenElemTable == 0) {
            printf("%s: failed malloc page.lenElemSet.\n", __func__);
            return false;
        } // Req Condition

        page->elemTable = calloc(page->lenElemTable, sizeof(KeyVal));
        if (page->elemTable == NULL) {
            printf("%s: failed malloc page.elemSet.\n", __func__);
            return false;
        } // Req Condition

        TEMPO_SetElemPublicTable(page->lenElemTable, page->elemTable);

        for (int i = 0; i < page->lenElemTable; i++) {
            const cJSON* elem_json = cJSON_GetArrayItem(elemTable_json, i);
            if (elem_json == NULL) {
                printf("%s: failed malloc page.elemSet.\n", __func__);
                return false;
            } // Req Condition
            page->elemTable[i].key = strdup(elem_json->string);
            if (page->elemTable[i].key == NULL) {
                printf("%s: failed malloc page.elemSet.\n", __func__);
                return false;
            } // Req Condition
            page->elemTable[i].val = TEMPO_CreateElem(elem_json); // malloc
            if (page->elemTable[i].val == NULL) {
                printf("%s: failed malloc page.elemSet.\n", __func__);
                return false;
            } // Req Condition
        }

        TEMPO_SetElemPublicTable(0, NULL);
    }
    if (cJSON_ExistKey(page_json, key = "anchor")) {
        if (cJSON_LoadFromObj(page_json, key, JSM_INT, &page->anchor) == false) {
            printf("%s: failed malloc page.anchor.\n", __func__);
            return false;
        }
    }
    if (cJSON_ExistKey(page_json, key = "src")) {
        if (cJSON_LoadFromObj(page_json, key, JSM_RECT, &page->src_rect) == true) {
            page->src = &page->src_rect;
        }
        else {
            printf("%s: failed malloc page.src.\n", __func__);
            return false;
        }
    }
    if (cJSON_ExistKey(page_json, key = "color")) {
        if (cJSON_LoadFromObj(page_json, key, JSM_COLOR, &page->color) == false) {
            printf("%s: failed malloc page.color.\n", __func__);
            return false;
        }
    }
    return true;
}
static bool TEMPO_CreatePage_CK(const Page* page) {
    for (int i = 0; i < page->lenElemTable; i++) {
        if (page->elemTable[i].key == NULL || page->elemTable[i].val == NULL) {
            printf("%s: elemSet[%d] == NULL.\n", __func__, i);
            return false;
        }
    }
    return true;
}
Page* TEMPO_DeletePage(Page* page) {
    if (page != NULL) {
        if (page->elemTable != NULL) {
            for (int i = 0; i < page->lenElemTable; i++) {
                if (page->elemTable[i].key != NULL) {
                    free(page->elemTable[i].key); // free
                    page->elemTable[i].key = NULL;
                }
                if (page->elemTable[i].val != NULL) {
                    page->elemTable[i].val = TEMPO_DeleteElem(page->elemTable[i].val); // free
                }
            }
            free(page->elemTable);
            page->elemTable = NULL;
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
static bool TEMPO_RenewPage_DstRect(Page* page) {
    return SDL_LoadDstRectAligned(
        &page->dst_rect,
        NULL,
        page->src,
        NULL,
        NULL,
        page->anchor
    );
}
bool TEMPO_RenewPage(Page *page) {
    // Req Condition
    if (page == NULL) {
        DEBUG_SendMessageR("%s: page not exists.\n", __func__);
        return false;
    }

    if (TEMPO_RenewPage_DstRect(page) == false) {
        DEBUG_SendMessageR("%s: TEMPO_RenewPage_DstRect(page) == false.\n", __func__);
        return false;
    }

    //
    TEMPO_SetElemPublicBck(&page->dst_rect);
    {
        DEVICE_SetMouseLeftTrig(NULL);
        for (int i = 0; i < page->lenElemTable; i++) {
            const bool renew = TEMPO_RenewElem(page->elemTable[i].val);
            if (renew == false) {
                DEBUG_SendMessageR("%s: renew == false.\n", __func__);
                return false;
            }
        }
    }
    TEMPO_SetElemPublicBck(NULL);
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64);
    SDL_RenderFillRect(renderer, NULL);

    SDL_SetRenderSDLColor(renderer, page->color);
    SDL_RenderFillRect(renderer, &page->dst_rect);

    for (int i = 0; i < page->lenElemTable; i++) {
        const bool draw = TEMPO_DrawElem(page->elemTable[i].val);
        if (draw == false) {
            DEBUG_SendMessageR("%s: draw[%d] == false.\n", __func__, i);
            return false;
        }
    }
    return true;
}