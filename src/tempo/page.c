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


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreatePage_RK(Page* page, const cJSON* page_json) {
    memset(page, 0, sizeof(Page));
    const char* key;
    if (cJSON_ExistKey(page_json, key = "elemTable")) {
        TEMPO_SetElemPublicTable(&page->elemTable);

        const cJSON* table_json = cJSON_GetObjectItem(page_json, key);
        REQ_CONDITION(table_json != NULL, return false);
        REQ_CONDITION(BASIC_CreateTable(&page->elemTable, table_json, TEMPO_CreateElem), return false);

        TEMPO_SetElemPublicTable(NULL);
    }
    if (cJSON_ExistKey(page_json, key = "anchor")) {
        REQ_CONDITION(cJSON_Load(page_json, key, JSM_INT, &page->anchor), return false);
    }
    if (cJSON_ExistKey(page_json, key = "src")) {
        REQ_CONDITION(cJSON_Load(page_json, key, JSM_FRECT, &page->src_rect), return false);
        page->src = &page->src_rect;
    }
    if (cJSON_ExistKey(page_json, key = "color")) {
        REQ_CONDITION(cJSON_Load(page_json, key, JSM_COLOR, &page->color), return false);
    }
    return true;
}
Page* TEMPO_CreatePage(const cJSON* page_json) {
    REQ_CONDITION(page_json != NULL, return NULL);

    Page* page = calloc(1, sizeof(Page));
    REQ_CONDITION(page != NULL, return NULL);
    REQ_CONDITION(TEMPO_CreatePage_RK(page, page_json), page = TEMPO_DeletePage(page));
    return page;
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
        PERPH_SetMouseLeftTrig(NULL);
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

    SDL_SetRenderColor(renderer, page->color);
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