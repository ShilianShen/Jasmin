#include "page.h"


#include "../perph/perph.h"


// PAGE PARA ===========================================================================================================
const SDL_FRect* elemBckNow = NULL;


// PAGE ================================================================================================================
struct Page {
    Table elemTable;
    int anchor;
    SDL_FRect src_rect, *src;
    SDL_FRect dst_rect, *bck;
    SDL_Color color;
    Trig trigArray[SDL_SCANCODE_COUNT];
    char* para_string[SDL_SCANCODE_COUNT];
};


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreatePage_RK(Page* page, const cJSON* page_json) {
    memset(page, 0, sizeof(Page));

    const cJSON* table_json = cJSON_GetObjectItem(page_json, "elemTable");
    REQ_CONDITION(table_json != NULL, return false);
    TEMPO_SetElemTableNow(&page->elemTable);
    REQ_CONDITION(BASIC_CreateTable(&page->elemTable, table_json, TEMPO_CreateElem), return false);

    cJSON_LoadByKey(page_json, "anchor", JSM_INT, &page->anchor);
    cJSON_LoadByKey(page_json, "color", JSM_COLOR, &page->color);
    if (cJSON_LoadByKey(page_json, "src", JSM_FRECT, &page->src_rect)) page->src = &page->src_rect;

    const cJSON* trigArray_json = cJSON_GetObjectItem(page_json, "trigArray");
    if (trigArray_json != NULL) {
        for (int i = 0; i < cJSON_GetArraySize(trigArray_json); i++) {
            const cJSON* trig_json = cJSON_GetArrayItem(trigArray_json, i);
            const int key = SDL_GetScancodeFromName(trig_json->string);
            REQ_CONDITION(key != SDL_SCANCODE_UNKNOWN, return false);

            const char* func_json = NULL;
            if (cJSON_LoadByKey(trig_json, "func", JSM_STRING, &func_json)) {
                page->trigArray[key].func = BASIC_GetTableValByKey(TEMPO_TrigFuncTable, func_json);
                REQ_CONDITION(page->trigArray[key].func != NULL, return false);
            }

            const char* para_json = NULL;
            if (cJSON_LoadByKey(trig_json, "para", JSM_STRING, &para_json)) {
                page->para_string[key] = strdup(para_json);
                REQ_CONDITION(page->para_string[key] != NULL, return false);
                page->trigArray[key].para = (TrigPara)page->para_string[key];
            }
        }
    }
    return true;
}
void *TEMPO_CreatePage(const cJSON *page_json) {
    REQ_CONDITION(page_json != NULL, return NULL);
    Page* page = calloc(1, sizeof(Page));
    REQ_CONDITION(page != NULL, return NULL);
    REQ_CONDITION(TEMPO_CreatePage_RK(page, page_json), page = TEMPO_DeletePage(page));
    return page;
}
void *TEMPO_DeletePage(void *page_void) {
    Page* page = page_void;
    if (page != NULL) {
        if (page->elemTable.kv != NULL) {
            BASIC_DeleteTable(&page->elemTable, TEMPO_DeleteElem);
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
        page->src != NULL ? page->src : &windowRect,
        NULL,
        NULL,
        page->anchor
    );
}
bool TEMPO_RenewPage(Page *page) {
    REQ_CONDITION(page != NULL, return false);
    REQ_CONDITION(TEMPO_RenewPage_DstRect(page), return false);

    TEMPO_SetElemBckNow(&page->dst_rect);
    PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, (Trig){0});
    BASIC_RenewTable(&page->elemTable, TEMPO_RenewElem);
    TEMPO_SetElemBckNow(NULL);

    if (trigAuthorize) {
        for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
            PERPH_SetBoardKeyTrig(i, page->trigArray[i]);
        }
    }
    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawPage(const Page* page) {
    REQ_CONDITION(page != NULL, return false);
    SDL_SetRenderColor(renderer, page->color);
    SDL_RenderFillRect(renderer, &page->dst_rect);
    REQ_CONDITION(BASIC_DrawTable(&page->elemTable, TEMPO_DrawElem), return false);
    return true;
}