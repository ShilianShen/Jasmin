#include "page.h"

// PAGE PARA ===========================================================================================================
const SDL_FRect* publicBck = NULL;


// PAGE ================================================================================================================
struct Page {
    Table elemTable;
    int anchor;
    SDL_FRect src_rect, *src;
    SDL_FRect dst_rect, *bck;
    SDL_Color color;
};

//
void TEMPO_PrintPage(const Page* page) {
    printf("TEMPO_Page:");
    if (page == NULL) {
        printf("NULL\n");
        return;
    }
    printf("%d\n", page->elemTable.len);
}
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

        page->elemTable.len = cJSON_GetArraySize(elemTable_json);
        if (page->elemTable.len == 0) {
            printf("%s: failed malloc page.lenElemSet.\n", __func__);
            return false;
        } // Req Condition

        page->elemTable.kv = calloc(page->elemTable.len, sizeof(KeyVal));
        if (page->elemTable.kv == NULL) {
            printf("%s: failed malloc page.elemSet.\n", __func__);
            return false;
        } // Req Condition

        TEMPO_SetElemPublicTable(&page->elemTable);
        for (int i = 0; i < page->elemTable.len; i++) {
            const cJSON* elem_json = cJSON_GetArrayItem(elemTable_json, i);
            if (elem_json == NULL) {
                printf("%s: failed malloc page.elemSet.\n", __func__);
                return false;
            } // Req Condition

            page->elemTable.kv[i].key = strdup(elem_json->string);
            if (page->elemTable.kv[i].key == NULL) {
                printf("%s: failed malloc page.elemSet.\n", __func__);
                return false;
            } // Req Condition

            page->elemTable.kv[i].val = TEMPO_CreateElem(elem_json); // malloc
            if (page->elemTable.kv[i].val == NULL) {
                printf("%s: failed malloc page.elemSet, %s.\n", __func__, page->elemTable.kv[i].key);
                return false;
            } // Req Condition
        }
        TEMPO_SetElemPublicTable(NULL);
    }
    if (cJSON_ExistKey(page_json, key = "anchor")) {
        if (cJSON_LoadFromObj(page_json, key, JSM_INT, &page->anchor) == false) {
            printf("%s: failed malloc page.anchor.\n", __func__);
            return false;
        }
    }
    if (cJSON_ExistKey(page_json, key = "src")) {
        if (cJSON_LoadFromObj(page_json, key, JSM_FRECT, &page->src_rect) == true) {
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
    for (int i = 0; i < page->elemTable.len; i++) {
        if (page->elemTable.kv[i].key == NULL || page->elemTable.kv[i].val == NULL) {
            printf("%s: elemSet[%d] == NULL.\n", __func__, i);
            return false;
        }
    }
    return true;
}
Page* TEMPO_DeletePage(Page* page) {
    if (page != NULL) {
        if (page->elemTable.kv != NULL) {
            for (int i = 0; i < page->elemTable.len; i++) {
                if (page->elemTable.kv[i].key != NULL) {
                    free(page->elemTable.kv[i].key); // free
                    page->elemTable.kv[i].key = NULL;
                }
                if (page->elemTable.kv[i].val != NULL) {
                    page->elemTable.kv[i].val = TEMPO_DeleteElem(page->elemTable.kv[i].val); // free
                }
            }
            free(page->elemTable.kv);
            page->elemTable.kv = NULL;
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
        for (int i = 0; i < page->elemTable.len; i++) {
            const bool renew = TEMPO_RenewElem(page->elemTable.kv[i].val);
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
static bool TEMPO_DrawPage_Frame(const Page* page) {
    const float X = page->dst_rect.x;
    const float Y = page->dst_rect.y;
    const float W = page->dst_rect.w;
    const float H = page->dst_rect.h;

    const float A = 2;
    const float B = 1;
    const float C = 8;
    const SDL_FRect rects[8] = {
        {X - A, Y - A, W + 2 * A, A + B},
        {X - A, Y - A, A + B, H + 2 * A},
        {X - A, Y + H - B, W + 2 * A, A + B},
        {X + W - B, Y - A, A + B, H + 2 * A},
        {X - C, Y - C, 2 * C, 2 * C},
        {X + W - C, Y - C, 2 * C, 2 * C},
        {X - C, Y + H - C, 2 * C, 2 * C},
        {X + W - C, Y + H - C, 2 * C, 2 * C},
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRects(renderer, rects, 8);

    return true;
}
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

    for (int i = 0; i < page->elemTable.len; i++) {
        const bool draw = TEMPO_DrawElem(page->elemTable.kv[i].val);
        if (draw == false) {
            DEBUG_SendMessageR("%s: draw[%d] == false.\n", __func__, i);
            return false;
        }
    }

    // TEMPO_DrawPage_Frame(page);
    return true;
}