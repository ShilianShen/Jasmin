#include "page.h"


bool PAGE_IfReady(const Page* page) {
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


static void PAGE_LoadName(Page* page, const char* name) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return;
    }
    if (name == NULL) {
        printf("%s: name not exists.\n", __func__);
        return;
    }

    //
    page->name = strdup(name); // malloc page.name

    // Req Condition
    if (page->name == NULL) {
        printf("%s: failed malloc page.name.\n", __func__);
    }
}
void PAGE_Load(Page* page, const char* name, const toml_table_t* tomlPage) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return;
    }
    if (name == NULL) {
        printf("%s: name not exists.\n", __func__);
        return;
    }
    if (tomlPage == NULL) {
        printf("%s: tomlPage not exists.\n", __func__);
        return;
    }

    //
    *page = (Page){0};

    //
    PAGE_LoadName(page, name);

    //
    const toml_array_t* tomlElems = toml_array_in(tomlPage, "elems");
    if (tomlElems == NULL) {printf("%s: tomlElems not exists.\n", __func__); return;}
    page->numElems = toml_array_nelem(tomlElems);

    //
    page->elems = malloc(page->numElems * sizeof(Elem*));
    for (int i = 0; i < page->numElems; i++) {
        const toml_table_t* tomlElem = toml_table_at(tomlElems, i);
        if (tomlElem != NULL) {
            page->elems[i] = TEMPO_CreateElemFromToml(tomlElem); // malloc
        }
    }
}
void PAGE_Unload(Page* page) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return;
    }
    if (page->name != NULL) {
        free(page->name);
        page->name = NULL;
    }
    if (page->elems != NULL) {
        for (int i = 0; i < page->numElems; i++) {
            if (page->elems[i] != NULL) {
                TEMPO_DestroyElem(page->elems[i]); // free
                page->elems[i] = NULL;
            }
        }
        free(page->elems);
        page->elems = NULL;
    }
}


void PAGE_Renew(Page* page) {
    // Req Condition
    if (page == NULL) {
        DEBUG_SendMessageR("%s: page not exists.\n", __func__);
        return;
    }

    //
    for (int i = 0; i < page->numElems; i++) {
        if (TEMPO_GetElemOk(page->elems[i])) {
            TEMPO_RenewElem(page->elems[i]);
        }
    }
}
void PAGE_Draw(const Page* page) {
    // Req Condition
    if (page == NULL) {
        DEBUG_SendMessageR("%s: page not exists.\n", __func__);
        return;
    }

    //
    for (int i = 0; i < page->numElems; i++) {
        if (TEMPO_GetElemOk(page->elems[i])) {
            TEMPO_DrawElem(page->elems[i]);
        }
    }
}