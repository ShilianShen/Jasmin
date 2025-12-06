
#include "tempo.h"
#include "elem.h"


static const SDL_FRect* elemBckNow = NULL;
static const Table* elemTableNow = NULL;


// ELEM ================================================================================================================
struct Elem {
    TEMPO_Type* type;
    int anchor;
    SDL_FRect gid_rect, *gid;
    SDL_FRect src_rect, *src;
    SDL_FRect dst_rect, *bck;
    SDL_Texture* texture;
};


// GET & SET ===========================================================================================================
bool TEMPO_SetElemBckNow(const SDL_FRect* bck) {
    elemBckNow = bck;
    return true;
}
bool TEMPO_SetElemTableNow(const Table* table) {
    elemTableNow = table;
    return true;
}


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreateElem_RK(Elem* elem, const cJSON *elem_json) {
    const cJSON* type_json = cJSON_GetArrayItem(elem_json, 0);
    REQ_CONDITION(type_json != NULL, return false);

    elem->type = TEMPO_CreateType(type_json);
    REQ_CONDITION(elem->type != NULL, return false);

    cJSON_LoadByKey(elem_json, "anchor", JSM_INT, &elem->anchor);

    const char* bck_json = NULL;
    if (cJSON_LoadByKey(elem_json, "gid", JSM_FRECT, &elem->gid_rect)) elem->gid = &elem->gid_rect;
    if (cJSON_LoadByKey(elem_json, "src", JSM_FRECT, &elem->src_rect)) elem->src = &elem->src_rect;
    if (cJSON_LoadByKey(elem_json, "bck", JSM_STRING, &bck_json)) {
        REQ_CONDITION(elemTableNow != NULL, return false);
        Elem* another = BASIC_GetTableValByKey(*elemTableNow, bck_json);
        REQ_CONDITION(another != NULL, return false);
        elem->bck = &another->dst_rect;
    }

    return true;
}
void *TEMPO_CreateElem(const cJSON *elem_json) {
    REQ_CONDITION(elem_json != NULL, return NULL);
    Elem* elem = calloc(1, sizeof(Elem));
    REQ_CONDITION(elem != NULL, return NULL);
    REQ_CONDITION(TEMPO_CreateElem_RK(elem, elem_json), elem = TEMPO_DeleteElem(elem));
    return elem;
}
void *TEMPO_DeleteElem(void *elem_void) {
    Elem* elem = elem_void;
    if (elem == NULL) return elem;

    if (elem->type != NULL) {
        TEMPO_DeleteType(elem->type);
        elem->type = NULL;
    }

    free(elem);
    elem = NULL;
    return elem;
}


// RENEW ===============================================================================================================
bool TEMPO_RenewElem(void *elem_void) {
    Elem* elem = elem_void;
    REQ_CONDITION(elem != NULL, return false);

    elem->texture = TEMPO_RenewTypeTexture(elem->type);
    OPT_CONDITION(elem->texture != NULL, return true);

    SDL_LoadDstRectAligned(
        &elem->dst_rect,
        elem->texture,
        elem->src,
        elem->gid,
        elem->bck != NULL ? elem->bck : elemBckNow,
        elem->anchor
        );

    if (trigAuthorize) {
        const SDL_FPoint point = SDL_ScaleByTexture(SDL_StdPointByRect(PERPH_GetMousePos(), elem->dst_rect), elem->texture);
        TEMPO_RenewTypeTrig(elem->type, point);
    }

    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawElem(const void *elem_void) {
    const Elem *elem = elem_void;
    REQ_CONDITION(elem != NULL, return false);

    if (PERPH_GetMouseKeyInRect(PERPH_MOUSE_KEY_LEFT, elem->dst_rect)) DEBUG_FillRect(elem->dst_rect);

    const SDL_FRect dst = SDL_RoundFRect(elem->dst_rect);
    SDL_RenderTexture(renderer, elem->texture, elem->src, &dst);

    if (PERPH_GetMouseInRect(elem->dst_rect)) {
        DEBUG_DrawRect(elem->dst_rect);
        DEBUG_DrawLine((SDL_FPoint){elem->dst_rect.x, elem->dst_rect.y}, PERPH_GetMousePos());
    }

    return true;
}

