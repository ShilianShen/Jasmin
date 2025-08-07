#ifndef GENE_H
#define GENE_H


#include "../basic/basic.h"


typedef struct Gene Gene;


void GENE_Load();
void GENE_Unload();


Gene* GENE_FindGeneFromColor(SDL_Color color);
char* GENE_GetNameFromGene(const Gene* gene);


#endif //GENE_H