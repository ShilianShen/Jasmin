#include "tempo.h"
#include "elem.h"


static const SDL_FRect* elemBckNow = NULL;
static const Table* elemTableNow = NULL;


// ELEM ================================================================================================================
struct TEMPO_Elem {
    char* name;
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
static bool TEMPO_CreateElem_RK(TEMPO_Elem* elem, const cJSON *elem_json) {
    const cJSON* type_json = cJSON_GetObjectItem(elem_json, "type");
    REQ_CONDITION(type_json != NULL, return false);

    elem->type = TEMPO_CreateType(type_json);
    REQ_CONDITION(elem->type != NULL, return false);

    cJSON_LoadByKey(elem_json, "anchor", JSM_INT, &elem->anchor);

    const char* name_json = NULL;
    if (cJSON_LoadByKey(elem_json, "name", JSM_STRING, &name_json)) {
        elem->name = strdup(name_json);
        REQ_CONDITION(elem->name != NULL, return false);
    }

    const char* bck_json = NULL;
    if (cJSON_LoadByKey(elem_json, "gid", JSM_FRECT, &elem->gid_rect)) elem->gid = &elem->gid_rect;
    if (cJSON_LoadByKey(elem_json, "src", JSM_FRECT, &elem->src_rect)) elem->src = &elem->src_rect;
    if (cJSON_LoadByKey(elem_json, "bck", JSM_STRING, &bck_json)) {
        REQ_CONDITION(elemTableNow != NULL, return false);
        for (int i = 0; i < elemTableNow->len; i++) {
            TEMPO_Elem* another = elemTableNow->kv[i].val;
            if (another == NULL || another->name == NULL) continue;
            if (strcmp(another->name, bck_json) != 0) continue;
            elem->bck = &another->dst_rect;
        }
    }
    return true;
}
TEMPO_Elem *TEMPO_CreateElem(const cJSON *elem_json) {
    REQ_CONDITION(elem_json != NULL, return NULL);
    TEMPO_Elem* elem = calloc(1, sizeof(TEMPO_Elem));
    REQ_CONDITION(elem != NULL, return NULL);
    REQ_CONDITION(TEMPO_CreateElem_RK(elem, elem_json), elem = TEMPO_DeleteElem(elem));
    return elem;
}
TEMPO_Elem *TEMPO_DeleteElem(TEMPO_Elem *elem) {
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
bool TEMPO_RenewElem(TEMPO_Elem *elem) {
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
        TEMPO_RenewTypeTrig(elem->type, elem->dst_rect);
    }

    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawElem(const TEMPO_Elem* elem) {
    REQ_CONDITION(elem != NULL, return false);

    DEBUG_DrawRect(elem->dst_rect);

    if (PERPH_GetMouseKeyInRect(PERPH_MOUSE_KEY_LEFT, elem->dst_rect)) DEBUG_FillRect(elem->dst_rect);

    const SDL_FRect dst = SDL_RoundFRect(elem->dst_rect);
    SDL_RenderTexture(renderer, elem->texture, elem->src, &dst);

    if (PERPH_GetMouseInRect(elem->dst_rect)) {
        DEBUG_DrawRect(elem->dst_rect);
    }

    return true;
}

