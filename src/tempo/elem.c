
#include "tempo.h"
#include "elem.h"


static const SDL_FRect* publicBck = NULL;
static const Table* publicElemTable = NULL;


// ELEM ================================================================================================================
struct Elem {
    Type* type;
    int anchor;
    SDL_FRect gid_rect, *gid;
    SDL_FRect src_rect, *src;
    SDL_FRect dst_rect, *bck;

    Trig trig; char* para_string;
};


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
        REQ_CONDITION(publicElemTable != NULL, return false);
        Elem* another = BASIC_GetTableValByKey(*publicElemTable, bck_json);
        REQ_CONDITION(another != NULL, return false);
        elem->bck = &another->dst_rect;
    }

    const char* func_json = NULL; if (cJSON_LoadByKey(elem_json, "func", JSM_STRING, &func_json)) {
        const TrigFunc func = BASIC_GetTableValByKey(TEMPO_TrigFuncTable, func_json);
        REQ_CONDITION(func != NULL, return false);
        elem->trig.func = func;
    }
    const char* para_json = NULL; if (cJSON_LoadByKey(elem_json, "para", JSM_STRING, &para_json)) {
        elem->para_string = strdup(para_json);
        REQ_CONDITION(elem->para_string != NULL, return false);
        elem->trig.para = (TrigPara)elem->para_string;
    }

    if (TYPE_INFO_DETAIL[elem->type->id].trig.func != NULL) {
        elem->trig = TYPE_INFO_DETAIL[elem->type->id].trig;
        elem->trig.para = (TrigPara)elem;
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

    TEMPO_DeleteType(elem->type);
    if (elem->para_string != NULL) {
        free(elem->para_string);
        elem->para_string = NULL;
    }
    free(elem);
    elem = NULL;
    return elem;
}


// RENEW ===============================================================================================================
static bool TEMPO_RenewElem_Tex(Elem* elem) {
    if (elem->type->texture != NULL) {
        SDL_DestroyTexture(elem->type->texture);
        elem->type->texture = NULL;
    }
    REQ_CONDITION(TEMPO_RenewType(elem->type), return false);
    return true;
}
static bool TEMPO_RenewElem_DstRect(Elem *elem) {
    const bool result = SDL_LoadDstRectAligned(
        &elem->dst_rect,
        elem->type->texture,
        elem->src,
        elem->gid,
        elem->bck != NULL ? elem->bck : publicBck,
        elem->anchor
        );
    return result;
}
bool TEMPO_RenewElem(void *elem_void) {
    Elem* elem = elem_void;
    REQ_CONDITION(elem != NULL, return false);
    REQ_CONDITION(TEMPO_RenewElem_Tex(elem), return false);
    REQ_CONDITION(TEMPO_RenewElem_DstRect(elem), return false);

    const bool mouseIn = PERPH_GetMouseInRect(elem->dst_rect);
    const bool mouseLeftIn = PERPH_GetMouseKeyInRect(PERPH_MOUSE_KEY_LEFT, elem->dst_rect);

    if (mouseLeftIn) {
        DEBUG_SendMessageL("Elem:\n");
        DEBUG_SendMessageL("    typeId: %s\n", TYPE_INFO_DETAIL[elem->type->id].name);
        if (elem->trig.func != NULL) {
            DEBUG_SendMessageL("    trig: %s(%s)\n", BASIC_GetTableKeyByVal(TEMPO_TrigFuncTable, elem->trig.func), elem->trig.para);
        }
        DEBUG_SendMessageL("    dst: %s\n", SDL_GetStrFRect(elem->dst_rect));
    }

    if (elem->trig.sustain && mouseLeftIn) {
        PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, elem->trig.func != NULL ? elem->trig : BASIC_TrigPass);
    }
    if (elem->trig.sustain == false && mouseLeftIn && mouseIn) {
        PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, elem->trig.func != NULL ? elem->trig : BASIC_TrigPass);
    }

    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawElem(const void *elem_void) {
    const Elem *elem = elem_void;
    REQ_CONDITION(elem != NULL, return false);

    const bool mouseIn = PERPH_GetMouseInRect(elem->dst_rect);
    const bool mouseLeftIn = PERPH_GetMouseKeyInRect(PERPH_MOUSE_KEY_LEFT, elem->dst_rect);
    if (mouseLeftIn) DEBUG_FillRect(elem->dst_rect);

    const SDL_FRect dst = {
        roundf(elem->dst_rect.x),
        roundf(elem->dst_rect.y),
        roundf(elem->dst_rect.w),
        roundf(elem->dst_rect.h),
    };
    const bool result = SDL_RenderTexture(renderer, elem->type->texture, elem->src, &dst);
    if (mouseIn || mouseLeftIn) DEBUG_DrawRect(elem->dst_rect);

    return result;
}


// TRIG ================================================================================================================
void TEMPO_TrigFuncBool(const TrigPara para) {
    const Elem* elem = (Elem*)para;
    bool* now = elem->type->info.bool_.now;
    if (now != NULL) *now = !*now;
}
void TEMPO_TrigFuncSlid(const TrigPara para) {
    const Elem* elem = (Elem*)para;
    const SDL_FRect dst_rect = elem->dst_rect;
    const TypeSlidInfo* slid = &elem->type->info.slid;
    TrigFunc_Slid(slid, dst_rect);
}


// SET & GET ===========================================================================================================
bool TEMPO_SetElemPublicBck(const SDL_FRect* bck) {
    if (bck == NULL) {
        return false;
    }
    publicBck = bck;
    return true;
}
bool TEMPO_SetElemPublicTable(const Table* table) {
    publicElemTable = table;
    return true;
}