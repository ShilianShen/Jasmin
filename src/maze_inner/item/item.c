#include "item.h"


Item* itemSet = NULL;
int lenItemSet = 0;
static const char* itemSetPath = "../config/maze/itemSet.toml";


static void ITEM_PrintItemSet() {
    for (int i = 0; i < lenItemSet; i++) {
        const Item* item = &itemSet[i];
        printf("%04d: %s\n", i, item->name);
    }
}
void ITEM_Load() {
    toml_table_t* file = getToml(itemSetPath); // malloc
    if (file == NULL) {
        printf("%s, %s", __func__, itemSetPath);
        return;
    }
    const toml_array_t* itemSetToml = toml_array_in(file, "itemSet");
    lenItemSet = toml_array_nelem(itemSetToml);
    itemSet = malloc(sizeof(Item) * lenItemSet); // malloc
    if (itemSet == NULL) {
        printf("%s, %s", __func__, itemSetPath);
    }
    for (int i = 0; i < lenItemSet; i++) {
        const toml_table_t* itemToml = toml_table_at(itemSetToml, i);
        if (itemToml == NULL) {
            printf("%s, %s", __func__, itemSetPath);
        }
        toml_datum_t nameToml = toml_string_in(itemToml, "name");
        if (nameToml.ok == true) {
            itemSet[i].name = strdup(nameToml.u.s); // malloc
            if (itemSet[i].name == NULL) {
                printf("%s, %s", __func__, itemSetPath);
            }
        }
    }
    toml_free(file); // free
    ITEM_PrintItemSet(); //
}
void ITEM_Unload() {
    if (itemSet != NULL) {
        for (int i = 0; i < lenItemSet; i++) {
            if (itemSet[i].name != NULL) {
                free(itemSet[i].name); // free
                itemSet[i].name = NULL;
            }
        }
        free(itemSet);
        itemSet = NULL;
        lenItemSet = 0;
    }
}