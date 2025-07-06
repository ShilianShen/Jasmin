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


void PAGE_Init(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    *page = (Page){0};
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
        ELEM_Init(&page->elems[i]);
    }
}
void PAGE_Deinit(const Page* page) {
    // Opt Condition
    if (page != NULL) {
        for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
            ELEM_Deinit(&page->elems[i]);
        }
    }
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
static void PAGE_LoadElems(Page* page, const toml_array_t* tomlElems) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (tomlElems == NULL) {printf("%s: tomlElems not exists.\n", __func__); return;}

    // Req Condition
    const int tomlElemsVolume = toml_array_nelem(tomlElems);
    if (tomlElemsVolume >= PAGE_ELEM_VOLUME) {
        printf("%s: tomlPage[%s].tomlElems overflows.\n", __func__, page->name);
        return;
    }

    //
    for (int i = 0; i < tomlElemsVolume; i++) {
        const toml_table_t* tomlElem = toml_table_at(tomlElems, i);
        // Opt Condition
        if (tomlElem != NULL) {
            ELEM_Load(&page->elems[i], tomlElem, i+1);
        }
    }
}
void PAGE_Load(Page* page, const char* name, const toml_table_t* tomlPage) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return;}
    if (tomlPage == NULL) {printf("%s: tomlPage not exists.\n", __func__); return;}

    //
    PAGE_LoadName(page, name);
    PAGE_LoadElems(page, toml_array_in(tomlPage, "elems"));
}
void PAGE_Unload(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    if (page->name != NULL) {free(page->name); page->name = NULL;}
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {ELEM_Unload(&page->elems[i]);}
}
void PAGE_Renew(Page* page) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return;
    }

    //
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
        if (page->elems[i].on == true) {
            ELEM_Renew(&page->elems[i]);
        }
    }
}
void PAGE_Draw(const Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
        if (page->elems[i].on == true) {
            ELEM_Draw(&page->elems[i]);
        }
    }
}