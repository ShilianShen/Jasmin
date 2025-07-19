#include "gene.h"


struct Gene {
    char* name;
    SDL_Color color;
};


static int lenGeneSet;
static Gene* geneSet;


static void GENE_LoadGene(Gene* gene, const toml_table_t* tomlGene) {
    // Req Condition
    if (gene == NULL) {
        printf("%s: gene not exists.\n", __func__);
        return;
    }
    if (tomlGene == NULL) {
        printf("%s: failed to read class.\n", __func__);
        return;
    }

    const toml_datum_t tomlName = toml_string_in(tomlGene, "name");
    if (tomlName.ok == false) {
        printf("%s: failed to read class.\n", __func__);
        return;
    }

    const toml_array_t* tomlColor = toml_array_in(tomlGene, "color");
    if (tomlColor == NULL) {
        printf("%s: failed to read class.\n", __func__);
        return;
    }

    const toml_datum_t tomlR = toml_int_at(tomlColor, 0);
    const toml_datum_t tomlG = toml_int_at(tomlColor, 1);
    const toml_datum_t tomlB = toml_int_at(tomlColor, 2);
    if (tomlR.ok == false || tomlG.ok == false || tomlB.ok == false) {
        printf("%s: failed to read class.\n", __func__);
    }

    //
    gene->name = strdup(tomlName.u.s); // malloc
    gene->color = (SDL_Color){tomlR.u.i, tomlG.u.i, tomlB.u.i, 255};
}
static void GENE_UnloadGene(Gene* gene) {
    if (gene != NULL) {
        if (gene->name != NULL) {
            free(gene->name); // free
            gene->name = NULL;
        }
    }
}


static void GENE_LoadGeneSet(const char* path) {
    // Req Condition
    toml_table_t* tomlFile = getToml(path); // malloc
    if (tomlFile == NULL) {
        printf("%s: failed to read \"%s\".\n", __func__, path);
        return;
    }

    const toml_array_t* tomlGeneSet = toml_array_in(tomlFile, "geneSet");
    if (tomlGeneSet == NULL) {
        printf("%s: failed to read geneSet.\n", __func__);
        return;
    }

    //
    lenGeneSet = toml_array_nelem(tomlGeneSet);
    geneSet = malloc(lenGeneSet * sizeof(Gene)); // malloc
    if (geneSet == NULL) {
        printf("%s: failed to allocate memory for geneSet.\n", __func__);
    }

    for (int i = 0; i < lenGeneSet; i++) {
        // Req Condition
        const toml_table_t* tomlGene = toml_table_at(tomlGeneSet, i);
        if (tomlGene == NULL) {
            printf("%s: failed to read geneSet[%d].\n", __func__, i);
            continue;
        }

        //
        GENE_LoadGene(&geneSet[i], tomlGene);
    }

    //
    toml_free(tomlFile); // free
}
static void GENE_UnloadGeneSet() {
    if (geneSet != NULL) {
        for (int i = 0; i < lenGeneSet; i++) {
            GENE_UnloadGene(&geneSet[i]);
        }
        free(geneSet); // free
        geneSet = NULL;
    }
}


void GENE_Load() {
    static const char* path = "/Users/shilianshen/Documents/Oolong Press/Lo-Tri/src/maze/gene/geneSet.toml";
    GENE_LoadGeneSet(path);
}
void GENE_Unload() {
    GENE_UnloadGeneSet();
}


Gene* GENE_FindGeneFromColor(const SDL_Color color) {
    for (int i = 0; i < lenGeneSet; i++) {
        const bool r = geneSet[i].color.r == color.r;
        const bool g = geneSet[i].color.g == color.g;
        const bool b = geneSet[i].color.b == color.b;
        if (r && g && b) {
            return &geneSet[i];
        }
    }
    return NULL;
}